#pragma once
#include "PathComponents.h"
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

                if(cache.closest_index >= cache.path.size() - 1)
                {
                    break;
                }

                cache.robot_properties.odom_controller->getModel()->tank(speeds.target_left.convert(rpm) / 280, speeds.target_right.convert(rpm) / 280);
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

        PathManager(RobotProperties properties)
        {
            robot_props = properties;
        }

        void generatePath(std::string path_name, GenerationParameters parameters, std::vector<RawPoint> path_points)
        {
            RawPath newPath(path_points);
            newPath = injectPoints(newPath);
            newPath = smoothPath(newPath, parameters);
            Path finishedPath = processPath(newPath, robot_props, parameters);
            finishedPath.setName(path_name);
            stored_paths.insert(pair<std::string, Path>(finishedPath.getName(), finishedPath));
        }

        void traversePath(std::string path_name, TraversalParameters parameters)
        {
            Path path = stored_paths[path_name];
            PathTraverser traverser(path, parameters, robot_props);
            traverser.traversePath();
        }

        void dumpPath(std::string path_name)
        {
            Path path = stored_paths[path_name];
            printf("\n-----\nDUMPING PATH: ");
            printf(path.getName().c_str());

            std::string x_coords = "\n X [";
            std::string y_coords = "\n Y [";
            std::string distances =  "\n D [";
            std::string target_vels = "\n V [";
            std::string curvatures = "\n C [";

            for(int i = 0; i < path.size(); i++)
            {
                x_coords += to_string(path.at(i).x_pos.convert(inch)) + ",";
                y_coords += to_string(path.at(i).y_pos.convert(inch)) + ",";
                distances += to_string(path.at(i).distance.convert(inch)) + ",";
                target_vels += to_string(path.at(i).target_velocity.convert(mps)) + ",";
                curvatures += to_string(path.at(i).curvature) + ",";
            }

            x_coords.replace(x_coords.length() - 1, 1, "]");
            y_coords.replace(y_coords.length() - 1, 1, "]");
            distances.replace(distances.length() - 1, 1, "]");
            target_vels.replace(target_vels.length() - 1, 1, "]");
            curvatures.replace(curvatures.length() - 1, 1, "]");

            printf((x_coords + y_coords + distances + target_vels + curvatures).c_str());
            printf("\n-----");
        }
};