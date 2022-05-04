#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/ROBOT.hpp"
#include <cfloat>

QSpeed RateLimiter::getLimited(QSpeed input, QAcceleration limit)
{
    double time_change = pros::millis() - last_called;
    last_called = pros::millis();
    QSpeed max_velocity  = (last_output.convert(mps) + (time_change / 1000) * limit.convert(mps2)) * mps;

    QSpeed output;
    if(input > max_velocity) { output = max_velocity; }
    else{ output = input; }

    last_output = output;
    return output;
}

PathTraverser::PathTraverser(Path path_to_traverse, const RobotProperties robot) : robot_properties(robot), path(path_to_traverse)
{
    //Everything else is default-initialized.
}

void PathTraverser::updatePosition()
{
    points.current_position = imu_odometer.getPosition();
    PRINT("COORDINATES // X: " + std::to_string(points.current_position.x.convert(foot)) + " Y: " + std::to_string(points.current_position.y.convert(foot)));
    PRINT("HEADING // RAW: " + std::to_string(points.current_position.theta.convert(degree))); 
}

void PathTraverser::findClosestPoint()
{
    QLength curr_closest_distance (DBL_MAX * meter);
    int curr_closest_index = points.closest_index;
    int ceil_index;

    if(Vector2D::dist(points.current_position, path.end()) <= path.lookahead_distance) 
    {
        ceil_index = path.size();
    }
    else
    {
        ceil_index = points.lookahead_index + 2;
    }

    for(int i = curr_closest_index; i < std::min(path.size() - 1, ceil_index); i++)
    {
        QLength distance = Vector2D::dist(points.current_position, path.at(i));
        if(distance.convert(meter) < curr_closest_distance.convert(meter))
        {
            curr_closest_distance = distance;
            curr_closest_index = i;
        }
    }

    points.closest_index = curr_closest_index;
    points.closest_point = path.at(curr_closest_index);

    PRINT("CP: " + std::to_string(points.closest_index + 1) + "/" + std::to_string(path.size()));
    PRINT("CPC: (" + std::to_string(points.closest_point.x_pos.convert(foot)) + ", " + std::to_string(points.closest_point.y_pos.convert(foot)) + ")");
}

double PathTraverser::findIntersect(Vector2D start, Vector2D end, Vector2D pos, QLength lookahead)
{
    Vector2D d = end - start;
    Vector2D f = start - pos;

    double a = d.dot(d);
    double b = 2 * f.dot(d);
    double c = f.dot(f) - (SQ(lookahead.convert(meter)));
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

        if(t2 >= 0 && t2 <= 1)
        {
            return t2;
        }

        if(t1 >= 0 && t1 <= 1)
        {
            return t1;
        }

        return -1.0; //No intersection
    }
}

void PathTraverser::calculateLookahead()
{   

    int previous_intersection_index = 0;
    double previous_t_value = 0;

    if(points.lookahead_index == 0 && Vector2D::dist(points.current_position, path.end()) < path.lookahead_distance) 
    {
      points.lookahead_index = path.size() - 2;
      previous_t_value = 1;
    }

    for(int i = std::max(points.lookahead_index, points.closest_index); i < path.size() - 1; i++)
    {
        Vector2D start (path.at(i));
        Vector2D end (path.at(i+1));
        Vector2D pos (points.current_position);
        double t_value = findIntersect(start, end, pos, path.lookahead_distance);
        double fractional_index = t_value + i;

        if(t_value < 0 || fractional_index <= points.lookahead_index) { continue; }
        else if(i > points.lookahead_index || t_value > previous_t_value)
        {   
            int cache = points.lookahead_index;
            points.lookahead_index = i;
            previous_t_value = t_value;

            Vector2D start = path.at(points.lookahead_index);
            Vector2D end = path.at(points.lookahead_index + 1);
            Vector2D d = end - start;
            Vector2D final_point = start + (d * previous_t_value);

            if(Vector2D::dist(final_point, end) > Vector2D::dist(points.current_position, end))
            {
                points.lookahead_index = cache;
                continue;
            }

            if(previous_intersection_index > 0)
            {
                break;  
            }

            previous_intersection_index = i;

            if(previous_intersection_index > 0 && Vector2D::dist(path.at(i), path.at(previous_intersection_index)) >= path.lookahead_distance * 2)
            {
                break;
            }
        }
    }
    
    Vector2D start = path.at(points.lookahead_index);
    Vector2D end = path.at(points.lookahead_index + 1);
    Vector2D d = end - start;
    Vector2D final_point = start + (d * previous_t_value);
    points.lookahead_point = final_point;
    projectLookahead();

    PRINT("LP: (" + std::to_string(points.lookahead_point.x_component.convert(foot)) + ", " + std::to_string(points.lookahead_point.y_component.convert(foot)) + ")");
    PRINT("LPI: " + std::to_string(points.lookahead_index + 1));
}

void PathTraverser::projectLookahead()
{
    Vector2D ray = points.lookahead_point - points.current_position;
    ray = ray.normalize();
    ray = ray * path.lookahead_distance.convert(meter);
    ray = ray + points.current_position;
    points.lookahead_point = ray;
}

void PathTraverser::calculateCurvature()
{
    Vector2D difference = points.lookahead_point - points.current_position;
    double alpha = std::atan2(difference.y_component.convert(meter), difference.x_component.convert(meter));
    double beta = constrainAngle180(points.current_position.theta).convert(radian) - alpha;
    double curvature = (2 * std::sin(beta)) / (path.lookahead_distance.convert(meter));
    outputs.curvature = conditions.end_within_lookahead ? 0 : -curvature;
    PRINT("Curvature: " + std::to_string(outputs.curvature));
}

void PathTraverser::calculateWheelSpeeds()
{
    QSpeed point_velocity = points.closest_point.velocity; 
    QSpeed target_velocity = point_velocity;
    double k_constant = path.k_constant;
    
    if(conditions.is_on_path)
    {
        target_velocity = std::min(target_velocity.convert(mps), k_constant / std::abs(outputs.curvature)) * mps;
    }
    else
    {
        target_velocity = std::min(robot_properties.max_velocity.convert(mps), k_constant / std::abs(outputs.curvature)) * mps;
    }

    target_velocity = std::max(target_velocity, robot_properties.min_velocity);

    target_velocity = limiter.getLimited(target_velocity, robot_properties.max_acceleration);

    QSpeed left_velocity = (target_velocity.convert(mps) * (2.0 + robot_properties.track_width.convert(meter) * outputs.curvature) / 2.0) * mps;
    QSpeed right_velocity = (target_velocity.convert(mps) * (2.0 - robot_properties.track_width.convert(meter) * outputs.curvature) / 2.0) * mps;

    QAngularSpeed left_wheels = (left_velocity / (M_PI * robot_properties.wheel_diam)) * 360_deg;
    QAngularSpeed right_wheels = (right_velocity / (M_PI * robot_properties.wheel_diam)) * 360_deg;

    PRINT("Point velocity (MPS): " + std::to_string(point_velocity.convert(mps)));
    PRINT("Target velocity (MPS): " + std::to_string(target_velocity.convert(mps)));
    PRINT("Linear velocities (MPS): " + std::to_string(left_velocity.convert(mps)) + " " + std::to_string(right_velocity.convert(mps)));
    PRINT("Angular velocities (RPM): " + std::to_string(left_wheels.convert(rpm)) + " " + std::to_string(right_wheels.convert(rpm)));

    outputs.target_speeds = WheelSpeeds {left_wheels, right_wheels};  
}

void PathTraverser::updateConditions()
{
    conditions.is_on_path = Vector2D::dist(points.current_position, points.closest_point) <= path.lookahead_distance;

    QAngle angle_to_end = angleToPoint(path.end(), points.current_position, points.current_position.theta).abs();
    conditions.end_within_lookahead = Vector2D::dist(points.closest_point, path.end()) < path.lookahead_distance &&
                            Vector2D::dist(points.current_position, path.end()) < path.lookahead_distance;
    
    conditions.is_past_end = path.reversed ? angle_to_end < (M_PI / 2) * radian : angle_to_end > (M_PI / 2) * radian;
    conditions.distance_to_end = Vector2D::dist(points.current_position, path.end()).abs();
    conditions.is_finished = conditions.is_past_end && conditions.end_within_lookahead;
    
    PRINT("On path? " + std::to_string(conditions.is_on_path));
    PRINT("EWL? " + std::to_string(conditions.end_within_lookahead));
    PRINT("Angle/Distance to end: " + std::to_string(angle_to_end.convert(degree)) + " // " + std::to_string(conditions.distance_to_end.convert(inch)));
}

void PathTraverser::calculateAll()
{
    PRINT("------------ STEP! ------------");
    updatePosition();
    findClosestPoint();
    calculateLookahead();
    updateConditions();
    calculateCurvature();
    calculateWheelSpeeds();
}

bool PathTraverser::integrateCalculations()
{
    if(!path.reversed)
    {
        robot_properties.odom_controller->getModel()->tank(outputs.target_speeds.target_left.convert(rpm) / 200, outputs.target_speeds.target_right.convert(rpm) / 200);    
    }
    else
    {
        robot_properties.odom_controller->getModel()->tank(-outputs.target_speeds.target_left.convert(rpm) / 200, -outputs.target_speeds.target_right.convert(rpm) / 200);
    }

    if(conditions.is_finished) { return true; }
    else { return false; }
}

void PathTraverser::traversalLoop()
{
    reset();
    conditions.is_running = true;
    std::uint32_t delay_timestamp = pros::millis();
    while(true)
    {
        calculateAll();
        if(integrateCalculations()) { break; }

        pros::Task::delay_until(&delay_timestamp, 10);
    }
    conditions.is_running = false;
    robot_properties.odom_controller->getModel()->driveVector(0, 0); 
}

void PathTraverser::reset()
{
    TraversalPoints new_points; points = new_points;
    TraversalConditions new_conditions; conditions = new_conditions;
    TraversalOutputs new_outputs; outputs = new_outputs;
}

void PathTraverser::traversePath()
{
    if(conditions.is_running) { return; }
    traversalLoop();
}

void PathTraverser::traversePathAsync()
{   
    if(conditions.is_running) { return; }
    pros::Task traverseLoop([this] { traversalLoop(); }, TASK_PRIORITY_DEFAULT + 1);
}

bool PathTraverser::isSettled()
{
    return conditions.is_running;
}

void PathTraverser::waitUntilSettled()
{
    std::uint32_t delay_timestamp = pros::millis();
    while(conditions.is_running)
    {
        pros::Task::delay_until(&delay_timestamp, 10);
    }
}