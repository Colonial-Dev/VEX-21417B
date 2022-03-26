#pragma once
#include "PathComponents.h"
#include "InertialOdometry.h"
#include "PathCalculation.h"
#include "PathTraversal.h"

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
    PathPoint lookahead_point;
    int closest_index;
    int lookahead_index;
};

class PathTraverser
{
    private:

        TraversalCache cache;

    public:

        PathTraverser(Path path_to_traverse, TraversalParameters parameters, RobotProperties robot)
        {
            cache.path = path_to_traverse;
            cache.params = parameters;
            cache.robot_properties = robot;
        }

        void traversePath()
        {
            //fuckign GO
        }
};

class PathManager
{
    private:

        RobotProperties robot_props;
        std::vector<Path> stored_paths;

    public:

        PathManager(RobotProperties properties)
        {
            robot_props = properties;
        }
};