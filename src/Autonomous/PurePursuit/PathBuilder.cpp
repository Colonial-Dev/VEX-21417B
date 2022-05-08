#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Vector2D.hpp"

PathBuilder::PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller) : calling_manager(caller), robot_props(r_props)
{
    path_name = name;
    gen_params = g_params;
}

void PathBuilder::calculateCurvatures(Path& path)
{
    path.at(0).curvature = 0;
    for(int i = 1; i < path.size() - 1; i++)
    {
        double distance_alpha = Vector2D::dist(path.at(i), path.at(i-1)).convert(meter);
        double distance_beta = Vector2D::dist(path.at(i), path.at(i+1)).convert(meter);
        double distance_gamma = Vector2D::dist(path.at(i+1), path.at(i-1)).convert(meter);
        
        double side_product = distance_alpha * distance_beta * distance_gamma;
        double semi_perimeter = (distance_alpha + distance_beta + distance_gamma) / 2.0;
        double triangle_area = std::sqrt(semi_perimeter * (semi_perimeter - distance_alpha) * (semi_perimeter - distance_beta) * (semi_perimeter - distance_gamma));
        
        double radius = (side_product) / (4 * triangle_area);
        double curvature = 1/radius;
        curvature = std::isnan(curvature) ? 0 : curvature;

        path.at(i).curvature = curvature;
    }
    path.end().curvature = 0;
}

void PathBuilder::calculateVelocities(Path& path)
{
    path.end().velocity = 0_mps;
    for(int i = path.size() - 1; i > 0; i--)
    {
        PathPoint current_point = path.at(i);
        PathPoint prev_point = path.at(i-1);

        QSpeed desired_velocity = std::min(robot_props.max_velocity.convert(mps), (gen_params.initial_velocity_constant / current_point.curvature)) * mps;
        QLength distance = Vector2D::dist(current_point, prev_point);

        double limited_velocity = std::sqrt(SQ(current_point.velocity.convert(mps)) + (2 * robot_props.max_acceleration.convert(mps2) * distance.convert(meter)));
        QSpeed actual_velocity = std::min(desired_velocity.convert(mps), limited_velocity) * mps;

        path.at(i-1).velocity = actual_velocity;
    }
}

Path PathBuilder::calculatePath()
{
    std::uint32_t timestamp = pros::micros();
    Path computed_path;

    //Given the provided waypoints and settings, use quintic spline interpolation to generate the path's points.
    QuinticPathGenerator generator(path_waypoints, gen_params.spline_resolution, gen_params.smoothing_constant);
    std::vector<PathPoint> points = generator.getPath();

    //Transfer configuration to the new Path instance and perform final curvature/velocity calculations.
    computed_path.points = points;
    computed_path.name = path_name;
    computed_path.lookahead_distance = gen_params.lookahead_distance;
    computed_path.k_constant = gen_params.initial_velocity_constant;
    calculateCurvatures(computed_path);
    calculateVelocities(computed_path);

    std::uint32_t elapsed = pros::micros() - timestamp;
    PRINT("Finished generating path " + path_name + ", took " + std::to_string(elapsed) + " microseconds.");
    return computed_path;
}

PathBuilder PathBuilder::withRobotProperties(RobotProperties r_props)
{
    robot_props = r_props;
    return *this;
}

PathBuilder PathBuilder::withPoint(Waypoint point)
{
    Waypoint new_point = point;
    path_waypoints.push_back(new_point);
    return *this;
}

PathBuilder PathBuilder::withPoints(std::vector<Waypoint> points)
{
    for(int i = 0; i < points.size(); i++)
    {
        withPoint(points.at(i));
    }
    return *this;
}

PathBuilder PathBuilder::withOrigin()
{
    withPoint({0_ft, 0_ft, 0_deg});
    return *this;
}

PathBuilder PathBuilder::withCurrentPosition(IMUOdometer& odometer)
{   
    OdomState position = odometer.getPosition();
    withPoint({position.x, position.y, position.theta});
    return *this;
}

PathBuilder PathBuilder::isReversed(bool val)
{
    is_reversed = val;
    return *this;
}

PathBuilder PathBuilder::makeReversed()
{
    compute_reversed = true;
    return *this;
}

PathBuilder PathBuilder::withDebugDump()
{
    do_debug_dump = true;
    return *this;
}

void PathBuilder::generatePath()
{
    Path computed_path = calculatePath();
    computed_path.reversed = is_reversed;
    calling_manager.insertPath(computed_path);

    if(compute_reversed)
    {
        std::vector<Waypoint> reversed_path = path_waypoints;
        std::reverse(reversed_path.begin(), reversed_path.end());
        path_waypoints = reversed_path;

        Path computed_reversed_path = calculatePath();
        computed_reversed_path.name = path_name + "_rev";
        computed_reversed_path.reversed = true;
        calling_manager.insertPath(computed_reversed_path);
    }

    if(do_debug_dump) { calling_manager.dumpPath(path_name); }
}

