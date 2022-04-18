#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "okapi/squiggles/squiggles.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/PathObjects.hpp"
#include "robokauz/Autonomous/PurePursuit/PathFinder.hpp"

std::vector<squiggles::Pose> PathFinder::transformToCartesian(std::vector<Waypoint> waypoints)
{
    std::vector<squiggles::Pose> pose_points;

    for(int i = 0; i < waypoints.size(); i++)
    {
        Waypoint waypoint = waypoints.at(i);
        squiggles::Pose cartesian_point = {waypoint.y_pos.convert(meter), waypoint.x_pos.convert(meter), (90_deg - waypoint.heading).convert(radian)};
        pose_points.push_back(cartesian_point);
    }

    return pose_points;
}

std::vector<squiggles::ProfilePoint> PathFinder::generateSplinePath(std::vector<squiggles::Pose> waypoints)
{
    return generator.generate(waypoints);
}

std::vector<PathPoint> PathFinder::stripForExport(std::vector<squiggles::ProfilePoint> path)
{
    std::vector<PathPoint> stripped_path;

    for(int i = 0; i < path.size(); i++)
    {
        squiggles::ProfilePoint full_point = path.at(i);
        PathPoint stripped_point;

        stripped_point.x_pos = full_point.vector.pose.y * meter;
        stripped_point.y_pos = full_point.vector.pose.x * meter;
        stripped_point.heading = (full_point.vector.pose.yaw * radian) + 90_deg;
        stripped_point.left_velocity = full_point.wheel_velocities.at(0) * mps;
        stripped_point.right_velocity = full_point.wheel_velocities.at(1) * mps;

        stripped_path.push_back(stripped_point);
    }

    return stripped_path;
}

PathFinder::PathFinder(RobotProperties r_props) : 
constraints(squiggles::Constraints(r_props.max_velocity.convert(mps), r_props.max_acceleration.convert(mps2), r_props.max_acceleration.convert(mps2) * 2)),
generator(squiggles::SplineGenerator (constraints, std::make_shared<squiggles::TankModel>(r_props.track_width.convert(meter), constraints)))
{

}

Path PathFinder::generatePath(std::string path_name, std::vector<Waypoint> waypoints)
{
    Path new_path;
    std::uint32_t timestamp = pros::millis();
    std::vector<PathPoint> points = stripForExport(generateSplinePath(transformToCartesian(waypoints)));
    std::uint32_t elapsed = pros::millis() - timestamp;
    PRINT("Finished generating path " + path_name + ", took " + std::to_string(elapsed) + "microseconds.");

    new_path.points = points;
    new_path.setName(path_name);

    return new_path;
}