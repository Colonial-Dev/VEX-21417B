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

void PathBuilder::finalizeCurrentComponent()
{
    path_components.push_back(current_component);
    current_component.clear();
}

PathBuilder PathBuilder::withRobotProperties(RobotProperties r_props)
{
    robot_props = r_props;
    return *this;
}

PathBuilder PathBuilder::withPoint(Point point)
{
    RawPoint new_point = point;
    new_point.lookahead_distance = current_lookahead;
    current_component.push_back(new_point);
    return *this;
}

PathBuilder PathBuilder::withPoints(std::vector<Point> points)
{
    for(int i = 0; i < points.size(); i++)
    {
        withPoint(points.at(i));
    }
    return *this;
}

PathBuilder PathBuilder::withOrigin()
{
    withPoint({0_ft, 0_ft});
    return *this;
}

PathBuilder PathBuilder::withCurrentPosition(IMUOdometer& odometer)
{   
    OdomState position = odometer.getPosition();
    withPoint({position.x, position.y});
    return *this;
}

PathBuilder PathBuilder::withLookahead(QLength lookahead)
{
    current_lookahead = lookahead;
    return *this;
}

PathBuilder PathBuilder::segment()
{
    finalizeCurrentComponent();
    return *this;
}

PathBuilder PathBuilder::linkedSegment()
{
    RawPoint last_point = current_component.back();
    finalizeCurrentComponent();
    withPoint({last_point.x_pos, last_point.y_pos});
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
    finalizeCurrentComponent();

    std::vector<RawPath> path_segments;

    for(int i = 0; i < path_components.size(); i++)
    {
        if(path_components.at(i).size() != 0)
        {
            path_segments.push_back(path_components.at(i));
        }
    }

    for(int i = 0; i < path_segments.size(); i++)
    {
        RawPath processed_segment = injectPoints(path_segments.at(i), 6_in);
        processed_segment = smoothPath(processed_segment, gen_params);
        path_segments.at(i) = processed_segment;
    }

    RawPath combined_path;

    //Avoid expensive vector concatenation if we only have one segment
    if(path_segments.size() > 1) 
    {
        std::vector<RawPoint> all_points;
        
        for(int i = 0; i < path_segments.size(); i++)
        {
            all_points.insert(all_points.end(), 
            path_segments.at(i).points.begin(),
            path_segments.at(i).points.end());
        }

        combined_path = RawPath(all_points);
    }
    else
    {
        combined_path = path_segments.at(0);
    }

    Path computed_path = processPath(combined_path, robot_props, gen_params);
    computed_path.setName(path_name);
    calling_manager.insertPath(computed_path);

    if(computeReversed)
    {
        RawPath reversed_path = combined_path;
        std::reverse(reversed_path.points.begin(), reversed_path.points.end());

        Path computed_reversed_path = processPath(reversed_path, robot_props, gen_params);
        computed_reversed_path.setName(path_name + "_rev");
        computed_reversed_path.setReversed(true);
        calling_manager.insertPath(computed_reversed_path);
    }

    if(doDebugDump) { dumpFullPath(computed_path); }
}

