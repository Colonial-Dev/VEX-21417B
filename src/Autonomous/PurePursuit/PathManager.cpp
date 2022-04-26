#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>

PathManager::PathManager(RobotProperties properties)
{
    robot_props = properties;
}

int PathManager::getStatus()
{
    if(!current_traverser.has_value()) { return PurePursuitStatus::Idle; }
    else if(!current_traverser.value().isSettled()) { return PurePursuitStatus::Traversing; }
    else{ return PurePursuitStatus::Idle; }
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

PathBuilder PathManager::buildPath(std::string name, GenerationParameters g_params)
{
    return PathBuilder(name, g_params, robot_props, *this);
}

void PathManager::insertPath(Path path)
{
    stored_paths.insert(std::pair<std::string, Path>(path.name, path));
}

void PathManager::deletePath(std::string path_name)
{
    stored_paths.erase(path_name);
}

void PathManager::synchronousTraverse(std::string path_name)
{
    PathTraverser traverser = getTraverser(path_name);
    current_traverser.emplace(traverser);
    current_traverser.value().traversePath();
}

void PathManager::asynchronousTraverse(std::string path_name)
{
    PathTraverser traverser = getTraverser(path_name);
    traverser.traversePathAsync();
    current_traverser.emplace(traverser);
}

void PathManager::waitUntilSettled()
{
    if(current_traverser.has_value())
    {
        current_traverser.value().waitUntilSettled();
    }
}

PathTraverser PathManager::getTraverser(std::string path_name)
{
    Path path = stored_paths.at(path_name);
    return PathTraverser(path, robot_props);
}

void PathManager::dumpPath(std::string path_name)
{
    Path path = stored_paths.at(path_name);
    
    PRINT("\n-----\nDUMPING PATH: " + path.name);
    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";
    std::string velocity = "\n V [";
    std::string curvature = "\n C [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += std::to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += std::to_string(path.at(i).y_pos.convert(foot)) + ",";
        velocity += std::to_string(path.at(i).velocity.convert(mps)) + ",";
        curvature += std::to_string(path.at(i).curvature) + ", ";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");
    velocity.replace(velocity.length() - 1, 1, "]");
    curvature.replace(curvature.length() - 1, 1, "]");
    PRINT(x_coords + y_coords + velocity + curvature + "\n-----");
}