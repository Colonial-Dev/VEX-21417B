#pragma once
#include "PathComponents.h"
#include "PathCalculation.h"
#include "PathTraversal.h"

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