#pragma once
#include "PathComponents.h"
#include "PathCalculation.h"
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
            auto model = drive_train->getModel();
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

                model->tank(speeds.target_left.convert(rpm) / 200, speeds.target_right.convert(rpm) / 200);
                pros::Task::delay_until(&timestamp, 10);
            }
            model->driveVector(0, 0);
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

        PathTraverser loadPath(std::string path_name, TraversalParameters parameters)
        {
            Path path = stored_paths[path_name];
            return PathTraverser(path, parameters, robot_props);
        }
};