#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

PathBuilder::PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, QLength lookahead, PathManager& caller) : calling_manager(caller)
{
    path_name = name;
    gen_params = g_params;
    robot_props = r_props;
    lookahead_distance = lookahead;
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
    PathFinder pathfinder(robot_props);
    Path computed_path = pathfinder.generatePath(path_name, path_waypoints);
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

