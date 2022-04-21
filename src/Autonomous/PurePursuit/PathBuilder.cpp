#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

PathBuilder::PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller) : calling_manager(caller)
{
    path_name = name;
    gen_params = g_params;
    robot_props = r_props;
}

std::vector<squiggles::Pose> PathBuilder::transformToCartesian(std::vector<Waypoint> waypoints)
{
    std::vector<squiggles::Pose> pose_points;

    for(int i = 0; i < waypoints.size(); i++)
    {
        //CRUCIALLY IMPORTANT - Squiggles reacts poorly to negative angles and angles greater than 360 degrees,
        //so we need to constrain and convert angles to their equivalents within [0, 360). 
        //For example, -90 deg becomes 270 deg, and 540 deg becomes 180 deg.
        waypoints.at(i).heading = constrainAngle360(waypoints.at(i).heading);

        Waypoint waypoint = waypoints.at(i);
        squiggles::Pose cartesian_point = {waypoint.y_pos.convert(meter), waypoint.x_pos.convert(meter), (90_deg - waypoint.heading).convert(radian)};
        pose_points.push_back(cartesian_point);
    }

    return pose_points;
}

std::vector<squiggles::ProfilePoint> PathBuilder::generateSplinePath(std::vector<squiggles::Pose> waypoints)
{
    squiggles::Constraints constraints(robot_props.max_velocity.convert(mps), robot_props.max_acceleration.convert(mps2), robot_props.max_acceleration.convert(mps2) * 2);    
    squiggles::SplineGenerator generator(constraints, std::make_shared<squiggles::TankModel>(robot_props.track_width.convert(meter), constraints));
    return generator.generate(waypoints);
}

std::vector<PathPoint> PathBuilder::stripForExport(std::vector<squiggles::ProfilePoint> path)
{
    std::vector<PathPoint> stripped_path;

    for(int i = 0; i < path.size(); i++)
    {
        squiggles::ProfilePoint full_point = path.at(i);
        PathPoint stripped_point;

        stripped_point.x_pos = full_point.vector.pose.y * meter;
        stripped_point.y_pos = full_point.vector.pose.x * meter;
        stripped_path.push_back(stripped_point);
    }

    return stripped_path;
}

void PathBuilder::calculateCurvatures(Path& path)
{
    path.at(0).curvature = 0;
    for(int i = 1; i < path.size() - 1; i++)
    {
        double distance_alpha = interpointDistance(path.at(i), path.at(i-1)).convert(meter);
        double distance_beta = interpointDistance(path.at(i), path.at(i+1)).convert(meter);
        double distance_gamma = interpointDistance(path.at(i+1), path.at(i-1)).convert(meter);
        
        double side_product = distance_alpha * distance_beta * distance_gamma;
        double semi_perimeter = (distance_alpha + distance_beta + distance_gamma) / 2.0;
        double triangle_area = std::sqrt(semi_perimeter * (semi_perimeter - distance_alpha) * (semi_perimeter - distance_beta) * (semi_perimeter - distance_gamma));
        
        double radius = (side_product) / (4 * triangle_area);
        double curvature = std::isnan(curvature) ? 0 : 1 / radius;

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
        QLength distance = interpointDistance(current_point, prev_point);

        double limited_velocity = std::sqrt(SQ(current_point.velocity.convert(mps)) + (2 * robot_props.max_acceleration.convert(mps2) * distance.convert(meter)));
        QSpeed actual_velocity = std::min(desired_velocity.convert(mps), limited_velocity) * mps;

        path.at(i-1).velocity = actual_velocity;
    }
}

Path PathBuilder::calculatePath()
{
    std::uint32_t timestamp = pros::micros();
    Path computed_path;
    std::vector<PathPoint> points = stripForExport(generateSplinePath(transformToCartesian(path_waypoints)));


    computed_path.points = points;
    computed_path.name = path_name;
    computed_path.lookahead_distance = gen_params.lookahead_distance;
    computed_path.k_constant = gen_params.initial_velocity_constant;
    
    calculateCurvatures(computed_path);
    calculateVelocities(computed_path);

    std::uint32_t elapsed = pros::micros() - timestamp;
    PRINT("Finished generating path " + path_name + ", took " + std::to_string(elapsed) + " micros.");
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

PathBuilder PathBuilder::withPrealignment()
{
    hasPrealignment = true;
    return *this;
}

PathBuilder PathBuilder::makeReversed()
{
    computeReversed = true;
    return *this;
}

PathBuilder PathBuilder::withDebugDump()
{
    doDebugDump = true;
    return *this;
}

void PathBuilder::generatePath()
{
    Path computed_path = calculatePath();
    calling_manager.insertPath(computed_path);

    /*if(computeReversed)
    {
        RawPath reversed_path = combined_path;
        std::reverse(reversed_path.points.begin(), reversed_path.points.end());

        Path computed_reversed_path = processPath(reversed_path, robot_props, gen_params);
        computed_reversed_path.setName(path_name + "_rev");
        computed_reversed_path.setReversed(true);
        calling_manager.insertPath(computed_reversed_path);
    }*/

    if(doDebugDump) { dumpFullPath(computed_path); }
}

