#include <cfloat>
#pragma once

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

struct TraversalParameters
{
    double lookahead_distance;
};

struct TraversalCache
{
    TraversalParameters params;
    RobotProperties robot_properties;
    RobotPosition current_position;
    Path path;
    PathPoint closest_point;
    RawPoint lookahead_point;
    int closest_index;
    double lookahead_index;
};

void updateClosestPoint(TraversalCache& cache)
{
    double curr_closest_distance = DBL_MAX;
    int curr_closest_index = cache.closest_index;

    for(int i = curr_closest_index; i < cache.path.size(); i++)
    {
        double distance = interpointDistance(cache.current_position, cache.path.at(i));
        if(distance < curr_closest_distance)
        {
            curr_closest_distance = distance;
            curr_closest_index = i;
        }
    }

    cache.closest_index = curr_closest_index;
    cache.closest_point = cache.path.at(curr_closest_index);
}


double findIntersect(Vector& start, Vector& end, Vector& pos, double lookahead)
{
    Vector d = end.subtract(start);
    Vector f = start.subtract(pos);

    double a = d.dot(d);
    double b = 2 * f.dot(d);
    double c = f.dot(f) - (SQ(lookahead));
    double discriminant = SQ(b) - 4 * a * c;

    if(discriminant < 0)
    {
        return -1.0; //No intersection
    }
    else
    {
        discriminant = std::sqrt(discriminant);
        double t1  = (-b - discriminant) / (2 * a);
        double t2 = (-b + discriminant) / (2 * a);

        if(t1 >= 0 && t1 <= 1)
        {
            return t1;
        }

        if(t2 >= 0 && t2 <= 1)
        {
            return t2;
        }

        return -1.0; //No intersection
    }
}

void updateLookaheadPoint(TraversalCache& cache)
{
    for(int i = cache.lookahead_index; i < cache.path.size() - 1; i++)
    {
        Vector start (cache.path.at(i));
        Vector end (cache.path.at(i+1));
        Vector pos (cache.current_position);
        double t_value = findIntersect(start, end, pos, cache.params.lookahead_distance);
        double fractional_index = t_value + i;

        if(t_value < 0 || t_value < cache.lookahead_index) { continue; }
        else
        {   
            Vector d = end.subtract(start);
            PathPoint start_point = cache.path.at(i);

            double final_x = start_point.x_pos + (t_value * d.x_component);
            double final_y = start_point.y_pos + (t_value * d.y_component);

            RawPoint lookahead {final_x, final_y};
            cache.lookahead_point = lookahead;
            cache.lookahead_index = fractional_index;
            return;
        }
    }
}

double calculateCurvature(TraversalCache& cache)
{
    double lookahead_dist_x;
    double lookahead_dist_y;

    double a = -std::tan(cache.current_position.heading);
    double b = 1;
    double c = std::tan(cache.current_position.heading) * cache.current_position.x_pos - cache.current_position.y_pos;

    double cross_product = std::sin(cache.current_position.heading) * (cache.lookahead_point.x_pos - cache.current_position.x_pos) - 
           std::cos(cache.current_position.heading) * (cache.lookahead_point.y_pos - cache.current_position.y_pos);
    double side = sgnum(cross_product);

    lookahead_dist_x = std::abs(a * cache.lookahead_point.x_pos + b * cache.lookahead_point.y_pos + c) / std::sqrt(SQ(a) + SQ(b));

    double curvature = (2 * lookahead_dist_x) / SQ(cache.params.lookahead_distance); 
    return curvature * side;
}

WheelSpeeds calculateWheelSpeeds(TraversalCache &cache, double curvature)
{
    double point_velocity = cache.closest_point.target_velocity;
    double target_velocity = limiter.getLimited(point_velocity, cache.robot_properties.max_acceleration);
    double left_velocity = target_velocity * (2 + curvature * cache.robot_properties.track_width) / 2;
    double right_velocity = target_velocity * (2 - curvature * cache.robot_properties.track_width) / 2;
    return WheelSpeeds {left_velocity, right_velocity};
}