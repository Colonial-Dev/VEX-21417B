#include <cfloat>
#pragma once

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

struct TraversalParameters
{
    QLength lookahead_distance;
};

struct TraversalCache
{
    TraversalParameters params;
    RobotProperties robot_properties;
    OdomState current_position;
    Path path;
    PathPoint closest_point;
    RawPoint lookahead_point;
    int closest_index = 0;
    double lookahead_index = 0;
};

void updatePosition(TraversalCache& cache)
{
    cache.current_position = cache.robot_properties.odom_controller->getState();
}

void updateClosestPoint(TraversalCache& cache)
{
    QLength curr_closest_distance (DBL_MAX * meter);
    int curr_closest_index = cache.closest_index;

    for(int i = curr_closest_index; i < cache.path.size(); i++)
    {
        QLength distance = interpointDistance(cache.current_position, cache.path.at(i));
        if(distance.convert(meter) < curr_closest_distance.convert(meter))
        {
            curr_closest_distance = distance;
            curr_closest_index = i;
        }
    }

    cache.closest_index = curr_closest_index;
    cache.closest_point = cache.path.at(curr_closest_index);
    printf("\n CP:");
    printf(to_string(cache.closest_index).c_str());
    printf("/");
    printf(to_string(cache.path.size()).c_str());
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
        double t_value = findIntersect(start, end, pos, cache.params.lookahead_distance.convert(meter));
        double fractional_index = t_value + i;

        if(t_value < 0) { continue; }
        else
        {   
            Vector d = end.subtract(start);
            PathPoint start_point = cache.path.at(i);

            double final_x = start_point.x_pos.convert(meter) + (t_value * d.x_component.convert(meter));
            double final_y = start_point.y_pos.convert(meter) + (t_value * d.y_component.convert(meter));

            RawPoint lookahead {QLength (final_x * meter), QLength (final_y * meter)};
            printf("\n LPX:");
            printf(to_string(lookahead.x_pos.convert(meter)).c_str());
            printf("\n LPY:");
            printf(to_string(lookahead.y_pos.convert(meter)).c_str());
            cache.lookahead_point = lookahead;
            cache.lookahead_index = fractional_index;
            printf("\n LP:");
            printf(to_string(cache.lookahead_index).c_str());
            return;
        }
    }
}

double calculateCurvature(TraversalCache& cache)
{
    double lookahead_dist_x;
    double lookahead_dist_y;

    double a = -std::tan(cache.current_position.theta.convert(radian));
    double b = 1;
    double c = std::tan(cache.current_position.theta.convert(radian)) * cache.current_position.x.convert(meter) - cache.current_position.y.convert(meter);

    double cross_product = std::sin(cache.current_position.theta.convert(radian)) * (cache.lookahead_point.x_pos.convert(meter) - cache.current_position.x.convert(meter)) - 
           std::cos(cache.current_position.theta.convert(radian)) * (cache.lookahead_point.y_pos.convert(meter) - cache.current_position.y.convert(meter));
    double side = sgnum(cross_product);

    lookahead_dist_x = std::abs(a * cache.lookahead_point.x_pos.convert(meter) + b * cache.lookahead_point.y_pos.convert(meter) + c) / std::sqrt(SQ(a) + SQ(b));

    double curvature = (2 * lookahead_dist_x) / SQ(cache.params.lookahead_distance.convert(meter)); 
    return curvature * side;
}

WheelSpeeds calculateWheelSpeeds(TraversalCache &cache, double curvature)
{
    QSpeed point_velocity = cache.closest_point.target_velocity;
    printf("\n pV: ");
    printf(to_string(point_velocity.convert(mps)).c_str());
    QSpeed target_velocity = limiter.getLimited(point_velocity, cache.robot_properties.max_acceleration);
    printf("\n TV: ");
    printf(to_string(target_velocity.convert(mps)).c_str());
    QSpeed left_velocity = target_velocity * (2.0 + cache.robot_properties.track_width.convert(meter) * curvature) / 2.0;
    QSpeed right_velocity = target_velocity * (2.0 - cache.robot_properties.track_width.convert(meter) * curvature) / 2.0;

    QAngularSpeed left_wheels = (left_velocity / (1_pi * cache.robot_properties.wheel_diam)) * 360_deg;
    QAngularSpeed right_wheels = (right_velocity / (1_pi * cache.robot_properties.wheel_diam)) * 360_deg;

    printf("\n WVels:");
    printf(to_string(left_wheels.convert(rpm)).c_str());
    printf(" ");
    printf(to_string(right_wheels.convert(rpm)).c_str());

    return WheelSpeeds {left_wheels, right_wheels};
}