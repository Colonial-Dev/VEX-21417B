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

Path PathBuilder::calculatePath()
{
    PathFinder pathfinder(robot_props);
    Path computed_path = pathfinder.generatePath(path_name, path_waypoints);
    computed_path.lookahead_distance = gen_params.lookahead_distance;

    //Calculate path curvature per point
    computed_path.at(0).curvature = 0;
    computed_path.at(computed_path.size() - 1).curvature = 0;
    for(int i = 1; i < computed_path.size() - 1; i++)
    {
        PathPoint current_point = computed_path.at(i);
        PathPoint prev_point = computed_path.at(i - 1);
        PathPoint next_point = computed_path.at(i + 1);

        if(current_point.x_pos == prev_point.x_pos) 
        { 
            current_point.x_pos = QLength ((current_point.x_pos.convert(meter) + 0.001) * meter); 
        }

        double curr_x = current_point.x_pos.convert(meter);
        double curr_y = current_point.y_pos.convert(meter);
        double prev_x = prev_point.x_pos.convert(meter);
        double prev_y = prev_point.y_pos.convert(meter);
        double next_x = next_point.x_pos.convert(meter);
        double next_y = next_point.y_pos.convert(meter);

        double k_one = 0.5 * (SQ(curr_x) + SQ(curr_y) - SQ(prev_x) - SQ(prev_y)) / (curr_x - prev_x);

        double k_two = (curr_y - prev_y) / (curr_x - prev_x);

        double b = 0.5 * (SQ(prev_x) - 2 * prev_x * k_one + SQ(prev_y) - SQ(next_x) + 2 * next_x * k_one - SQ(next_y)) 
                         / (next_x * k_two - next_y + prev_y - prev_x * k_two);

        double a = k_one - k_two * b;
        
        double r = std::sqrt(SQ(curr_x - a) + SQ(curr_y - b));

        double curvature = 1/r; 

        if(std::isnan(curvature)) 
        { 
            curvature = 0; 
        }

        computed_path.at(i).curvature = curvature;
    }
    
    //Calculate initial max velocity per point
    for(int i = 0; i < computed_path.size(); i++)
    {
        computed_path.at(i).velocity = QSpeed (std::min(robot_props.max_velocity.convert(mps), gen_params.initial_velocity_constant / computed_path.at(i).curvature) * mps);
    }

    computed_path.at(computed_path.size() - 1).velocity = 0_mps;

    //Smooth deceleration velocities
    for(int i = computed_path.size() - 2; i >= 0; i--)
    {
        PathPoint current_point = computed_path.at(i);
        PathPoint next_point = computed_path.at(i+1);
        QLength distance = interpointDistance(current_point, next_point);
        computed_path.at(i).velocity = QSpeed (std::min(current_point.velocity.convert(mps), std::sqrt(SQ(next_point.velocity.convert(mps)) + 2 * robot_props.max_acceleration.convert(mps2) * distance.convert(meter))) * mps);
    }

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

