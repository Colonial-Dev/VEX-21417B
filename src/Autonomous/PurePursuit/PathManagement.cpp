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
        
        if(checkDistance(cache, 6_in)) { break; }

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

//Construct a new PathManager with the given robot properties.
PathManager::PathManager(RobotProperties properties)
{
    robot_props = properties;
}

//Generates a standard path from the given parameters and stores it in the PathManager instance.
void PathManager::generateStandardPath(std::string path_name, GenerationParameters parameters, std::vector<Vector> path_points)
{
    Path finished_path = generatePath(robot_props, parameters, path_points);
    finished_path.setName(path_name);
    stored_paths.insert(std::pair<std::string, Path>(finished_path.getName(), finished_path));
}

//Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
void PathManager::pathfindLinear()
{

}

//Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
void PathManager::pathfindDistance()
{

}

//Generates a temporary arc path from the robot's current position to a given point, then traverses it.
void PathManager::pathfindArc()
{

}

//Traverses a stored path, as identified by its name.
void PathManager::traverseStoredPath(std::string path_name, TraversalParameters parameters)
{
    Path path = stored_paths[path_name];
    PathTraverser traverser(path, parameters, robot_props);
    traverser.traversePath();
}

//Dumps a stored path to the terminal.
void PathManager::dumpStoredPath(std::string path_name)
{
    Path path = stored_paths[path_name];
    dumpFullPath(path);
}