#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"
#include "PathDumping.hpp"
#include "PathGeneration.hpp"
#include "PathTraversal.hpp"
#include <map>

class PathTraverser
{
    private:

        TraversalCache cache;

    public:

        PathTraverser(Path path_to_traverse, TraversalParameters parameters, RobotProperties robot);

        void traversePath();

        void simulatePath(OdomState position);
};

class PathManager
{
    private:

        RobotProperties robot_props;
        std::map<std::string, Path> stored_paths;

    public:

        //Construct a new PathManager with the given robot properties.
        PathManager(RobotProperties properties);

        //Generates a standard path from the given parameters and stores it in the PathManager instance.
        void generateStandardPath(std::string path_name, GenerationParameters parameters, std::vector<Vector> path_points);

        //Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
        void pathfindLinear();

        //Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
        void pathfindDistance();

        //Generates a temporary arc path from the robot's current position to a given point, then traverses it.
        void pathfindArc();

        //Traverses a stored path, as identified by its name.
        void traverseStoredPath(std::string path_name, TraversalParameters parameters);

        //Dumps a stored path to the terminal.
        void dumpStoredPath(std::string path_name);

};