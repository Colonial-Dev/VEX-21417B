#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/PathDumping.hpp"
#include "robokauz/Autonomous/PurePursuit/PathGeneration.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManagement.hpp"
#include "robokauz/Autonomous/PurePursuit/PathObjects.hpp"
#include "robokauz/Autonomous/PurePursuit/PathTraversal.hpp"
#include <map>

/// --- PathTraverser --

PathTraverser::PathTraverser(Path path_to_traverse, TraversalParameters parameters, RobotProperties robot)
{
    cache.path = path_to_traverse;
    cache.params = parameters;
    cache.robot_properties = robot;
}

void PathTraverser::traversePath()
{
    std::uint32_t delay_timestamp = pros::millis();
    std::uint32_t dt_timestamp = pros::millis();
    while(true)
    {
        updatePosition(cache);
        updateClosestPoint(cache);
        updateLookaheadPoint(cache);
        projectLookaheadPoint(cache);

        double curvature = calculateCurvature(cache);
        PRINT("\nCurvature: " + std::to_string(curvature));
        
        calculateWheelSpeeds(cache, curvature);
        
        if(cache.closest_index == cache.path.size() - 1) { break; }

        cache.robot_properties.odom_controller->getModel()->tank(cache.target_speeds.target_left.convert(rpm) / 200, cache.target_speeds.target_right.convert(rpm) / 200);

        pros::Task::delay_until(&delay_timestamp, 20);
    }
    cache.robot_properties.odom_controller->getModel()->driveVector(0, 0);
}

void PathTraverser::simulatePath(OdomState position)
{
    //TODO calculate + dump function outputs given hypothetical position
}

// --- PathManager ---

int PathManager::getStatus()
{
    return current_status;
}

void PathManager::setStatus(int new_status)
{
    current_status = new_status;
}

void PathManager::traversePath(Path path, TraversalParameters parameters)
{
    PathTraverser traverser(path, parameters, robot_props);
    setStatus(PurePursuitStatus::Traversing);
    traverser.traversePath();
    setStatus(PurePursuitStatus::Idle);
}

PathManager::PathManager(RobotProperties properties)
{
    robot_props = properties;
}

std::string PathManager::getPrettyStatus()
{
    switch(getStatus())
    {
        case PurePursuitStatus::Idle:
        {
            return "[#00ff00 IDLE#]";
        }
        case PurePursuitStatus::Traversing:
        {
            return "[#ffff00 TRAVERSING#]";
        }
    }
    return "[#ff0000 ERROR]";
}

void PathManager::generateStandardPath(std::string path_name, GenerationParameters parameters, std::vector<Vector> path_points)
{
    Path finished_path = generatePath(robot_props, parameters, path_points);
    finished_path.setName(path_name);
    stored_paths.insert(std::pair<std::string, Path>(finished_path.getName(), finished_path));
}

void PathManager::traverseStoredPath(std::string path_name, TraversalParameters parameters)
{
    Path path = stored_paths[path_name];
    traversePath(path, parameters);
}

void PathManager::dumpStoredPath(std::string path_name)
{
    Path path = stored_paths[path_name];
    dumpFullPath(path);
}