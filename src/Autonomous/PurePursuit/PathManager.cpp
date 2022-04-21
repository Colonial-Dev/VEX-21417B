#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>

PathManager::PathManager(RobotProperties properties)
{
    robot_props = properties;
}

PathBuilder PathManager::buildPath(std::string name, GenerationParameters g_params)
{
    return PathBuilder(name, g_params, robot_props, *this);
}

void PathManager::insertPath(Path path)
{
    stored_paths.insert(std::pair<std::string, Path>(path.name, path));
}

void PathManager::synchronousTraverse(std::string path_name)
{
    Path path = stored_paths.at(path_name);
    PathTraverser traverser(path, robot_props);
    traverser.traversePath();
}

PathTraverser PathManager::getTraverser(std::string path_name)
{
    Path path = stored_paths.at(path_name);
    return PathTraverser(path, robot_props);
}

Path PathManager::getPath(std::string path_name)
{
    return stored_paths.at(path_name);;
}

void PathManager::dumpPath(std::string path_name)
{
    Path path = stored_paths.at(path_name);
    dumpFullPath(path);
}