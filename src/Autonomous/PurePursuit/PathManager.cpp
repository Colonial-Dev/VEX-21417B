#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/ROBOT.hpp"
#include <map>

PathManager::PathManager(RobotProperties properties)
{
    robot_props = properties;
}

int PathManager::getStatus()
{
    if(!current_traverser.has_value()) { return PurePursuitStatus::Idle; }
    else if(!current_traverser.value().isSettled()) { return PurePursuitStatus::Idle; }
    else{ return PurePursuitStatus::Traversing; }
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
    stored_paths.insert_or_assign(path.name, path);
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
    current_traverser.emplace(traverser);
    current_traverser.value().traversePathAsync();
}

void PathManager::traverseLinear(Point point, bool backwards)
{   
    buildPath("TRANSIENT_PATH", {1, 5_in, 30, 1.0})
        .withRobotProperties({0.25_mps, 1.25_mps, 1.5_mps2, 12.0_in, 4.125_in, drive_controller})
        .withCurrentPosition(imu_odometer)
        .withPoint({point.x, point.y})
        .isReversed(backwards)
        .generatePath();
    
    synchronousTraverse("TRANSIENT_PATH");
}

void PathManager::traverseLinearAsync(Point point, bool backwards)
{
    buildPath("TRANSIENT_PATH", {1, 5_in, 30, 1.0})
        .withRobotProperties({0.25_mps, 1.25_mps, 1.5_mps2, 12.0_in, 4.125_in, drive_controller})
        .withCurrentPosition(imu_odometer)
        .withPoint({point.x, point.y})
        .isReversed(backwards)
        .generatePath();
    
    asynchronousTraverse("TRANSIENT_PATH"); 
}

void PathManager::traverseDistance(QLength distance)
{
    Vector2D position = imu_odometer.getPosition();
    QAngle heading = constrainAngle360(imu_odometer.getPosition().theta);
    heading = (distance < 0_m) ? 180_deg + heading : heading;
    Vector2D difference = {distance, heading};
    Vector2D target = position + difference;
    traverseLinear({target.x_component, target.y_component}, (distance < 0_m));
}

void PathManager::traverseDistanceAsync(QLength distance)
{
    Vector2D position = imu_odometer.getPosition();
    QAngle heading = constrainAngle360(imu_odometer.getPosition().theta);
    heading = (distance < 0_m) ? 180_deg + heading : heading;
    Vector2D difference = {distance, heading};
    Vector2D target = position + difference;
    traverseLinearAsync({target.x_component, target.y_component}, (distance < 0_m));
}

void PathManager::waitUntilSettled()
{
    if(current_traverser.has_value())
    {
        current_traverser.value().waitUntilSettled();
    }
}

void PathManager::forceAbort()
{
    if(current_traverser.has_value())
    {
        current_traverser.value().forceAbort();
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