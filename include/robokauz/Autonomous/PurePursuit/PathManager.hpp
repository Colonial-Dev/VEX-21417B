#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>

class PathManager
{
    private:

        RobotProperties robot_props;

        std::map<std::string, Path> stored_paths;

        //TODO incorporate current traverser instance into manager?

    public:

        PathManager(RobotProperties properties);

        PathBuilder buildPath(std::string name, GenerationParameters g_params);

        void insertPath(Path path);

        //Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
        void pathfindLinear();

        //Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
        void pathfindDistance();

        void synchronousTraverse(std::string path_name);

        PathTraverser getTraverser(std::string path_name);

        Path getPath(std::string path_name);

        void dumpPath(std::string path_name);
};