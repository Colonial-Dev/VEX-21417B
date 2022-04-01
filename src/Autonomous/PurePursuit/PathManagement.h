#pragma once
#include "PathComponents.h"
#include "PathDumping.h"
#include "PathGeneration.h"
#include "PathTraversal.h"
#include <map>

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
            std::uint32_t timestamp = pros::millis();
            while(true)
            {
                updatePosition(cache);
                updateClosestPoint(cache);
                updateLookaheadPoint(cache);
                double curvature = calculateCurvature(cache);
                WheelSpeeds speeds = calculateWheelSpeeds(cache, curvature);
                
                double distance = std::abs(interpointDistance(cache.current_position, cache.path.at(cache.path.size() - 1)).convert(inch));
                printf("\nDistance: ");
                printf(to_string(distance).c_str());
                if(distance < 6.0)
                {
                    break;
                }

                cache.robot_properties.odom_controller->getModel()->tank(speeds.target_left.convert(rpm) / 200, speeds.target_right.convert(rpm) / 200);
                pros::Task::delay_until(&timestamp, 20);
            }
            cache.robot_properties.odom_controller->getModel()->driveVector(0, 0);
        }
};

class PathManager
{
    private:

        RobotProperties robot_props;
        std::map<std::string, Path> stored_paths;

    public:

        //Construct a new PathManager with the given robot properties.
        PathManager(RobotProperties properties)
        {
            robot_props = properties;
        }

        //Generates a standard path from the given parameters and stores it in the PathManager instance.
        void generateStandardPath(std::string path_name, GenerationParameters parameters, std::vector<Vector> path_points)
        {
            Path finished_path = generatePath(robot_props, parameters, path_points);
            finished_path.setName(path_name);
            stored_paths.insert(pair<std::string, Path>(finished_path.getName(), finished_path));
        }

        //Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
        void pathfindLinear()
        {

        }

        //Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
        void pathfindDistance()
        {

        }

        //Generates a temporary arc path from the robot's current position to a given point, then traverses it.
        void pathfindArc()
        {

        }

        //Traverses a stored path, as identified by its name.
        void traverseStoredPath(std::string path_name, TraversalParameters parameters)
        {
            Path path = stored_paths[path_name];
            PathTraverser traverser(path, parameters, robot_props);
            traverser.traversePath();
        }

        //Dumps a stored path to the terminal.
        void dumpStoredPath(std::string path_name)
        {
            Path path = stored_paths[path_name];
            dumpFullPath(path);
        }
};