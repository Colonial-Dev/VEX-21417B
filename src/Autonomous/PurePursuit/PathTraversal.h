#include <cfloat>
#pragma once

struct TraversalParameters
{
    QLength lookahead_distance;
};

struct WheelSpeeds
{
    QAngularSpeed target_left;
    QAngularSpeed target_right;
};

struct TraversalCache
{
    TraversalParameters params;
    RobotProperties robot_properties;
    OdomState current_position;
    Path path;

    PathPoint closest_point;
    Vector lookahead_point;
    Vector projected_lookahead;
    int closest_index = 0;
    int lookahead_index = 0;

    WheelSpeeds target_speeds;
};

void updatePosition(TraversalCache& cache)
{
    cache.current_position = cache.robot_properties.odom_controller->getState();
    cache.current_position.theta = QAngle (inertial_sensor.get_rotation() * degree);
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

    PRINT "\nCP: " + to_string(cache.closest_index) + "/" + to_string(cache.path.size());
}


double findIntersect(Vector& start, Vector& end, Vector& pos, double lookahead)
{
    Vector d = end.subtract(start);
    Vector f = start.subtract(pos);

    double a = d.dot(d);
    double b = 2 * f.dot(d);
    double c = f.dot(f) - (SQ(lookahead));
    double discriminant = SQ(b) - (4.0 * (a * c));

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

        if(t_value < 0 || fractional_index <= cache.lookahead_index) { continue; }
        else
        {   
            Vector d = end.subtract(start);
            Vector d_scaled = d.scalarMult(t_value);            
            Vector final_point = start.add(d_scaled);

            Vector lookahead {final_point.x_component, final_point.y_component};
            cache.lookahead_point = lookahead;
            cache.lookahead_index = i;
            break;
        }
    }

    PRINT "\nLPX: " + to_string(cache.lookahead_point.x_component.convert(meter));
    PRINT "\nLPY: " + to_string(cache.lookahead_point.y_component.convert(meter));
    PRINT "\nLPI: " + to_string(cache.lookahead_index);
}

void projectLookaheadPoint(TraversalCache& cache)
{
    Vector ray = cache.lookahead_point.subtract(cache.current_position);
    ray = ray.normalize().scalarMult(cache.params.lookahead_distance.convert(meter));
    ray = ray.add(cache.current_position);
    cache.projected_lookahead = ray;
}

double calculateCurvature(TraversalCache& cache)
{
    Vector difference = cache.lookahead_point.subtract(cache.current_position);
    Vector lookahead = cache.lookahead_point;

    double bot_angle = (cache.current_position.theta).convert(radian);
    double a = -std::tan(bot_angle);
    double b = 1;
    double c = std::tan(bot_angle) * (cache.current_position.x.convert(meter) - cache.current_position.y.convert(meter));

    double lookahead_x = std::abs(a * lookahead.x_component.convert(meter) + b * lookahead.y_component.convert(meter) + c) / std::sqrt(SQ(a) + SQ(b));
    PRINT "\nLookX: " + to_string(lookahead_x);
    int side = sgnum(std::sin(bot_angle) * difference.x_component.convert(meter) - std::cos(bot_angle) * difference.y_component.convert(meter));

    double curvature = (2 * lookahead_x) / SQ(interpointDistance(cache.current_position, lookahead).convert(meter)); 
    return curvature * side;
}

bool checkDistance(TraversalCache& cache, QLength threshold = 6_in)
{
    double distance = std::abs(interpointDistance(cache.current_position, cache.path.at(cache.path.size() - 1)).convert(inch));
    PRINT "\nDistance: " + to_string(distance);
    return distance <= threshold.convert(inch);
}

void calculateWheelSpeeds(TraversalCache &cache, double curvature)
{    
    QSpeed point_velocity = cache.closest_point.target_velocity;
    QSpeed target_velocity = limiter.getLimited(point_velocity, cache.robot_properties.max_acceleration);
    QSpeed left_velocity = (target_velocity.convert(mps) * (2.0 + curvature * cache.robot_properties.track_width.convert(meter)) / 2.0) * mps;
    QSpeed right_velocity = (target_velocity.convert(mps) * (2.0 + curvature * cache.robot_properties.track_width.convert(meter)) / 2.0) * mps;

    QAngularSpeed left_wheels = (left_velocity / (1_pi * cache.robot_properties.wheel_diam)) * 360_deg;
    QAngularSpeed right_wheels = (right_velocity / (1_pi * cache.robot_properties.wheel_diam)) * 360_deg;
    left_wheels = QAngularSpeed (std::clamp(left_wheels.convert(rpm), -200.0, 200.0) * rpm);
    right_wheels = QAngularSpeed (std::clamp(right_wheels.convert(rpm), -200.0, 200.0) * rpm);

    PRINT "\npV: " + to_string(point_velocity.convert(mps));
    PRINT "\ntV: " + to_string(target_velocity.convert(mps));
    PRINT "\nCVels: " + to_string(left_velocity.convert(mps)) + " " + to_string(right_velocity.convert(mps));
    PRINT "\nWVels: " + to_string(left_wheels.convert(rpm)) + " " + to_string(right_wheels.convert(rpm));

    cache.target_speeds = WheelSpeeds {left_wheels, right_wheels};
}