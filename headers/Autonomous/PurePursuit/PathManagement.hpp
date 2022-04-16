#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>

namespace PurePursuitStatus
{
    enum PurePursuitStatus
    {
        Idle,
        Traversing
    };
}

/**
 * @brief Transient object used to mediate path traversal.
 */
class PathTraverser
{
    private:

        /**
         * @brief A default-initialized TraversalCache structure.
         */
        TraversalCache cache;

    public:

        /**
         * @brief Constructs a new Path Traverser object.
         * 
         * @param path_to_traverse The Path object to traverse.
         * @param robot A correctly-configured RobotProperties structure.
         */
        PathTraverser(Path path_to_traverse, RobotProperties robot);

        /**
         * @brief Traverses the path specified in the constructor.
         */
        void traversePath();

};

class PathManager
{
    private:

        /**
         * @brief A RobotProperties structure. Acquired in the constructor.
         */
        RobotProperties robot_props;

        /**
         * @brief A hashmap correlating the names and actual objects of stored Paths.
         */
        std::map<std::string, Path> stored_paths;

        /**
         * @brief The current status of the manager.
         */
        int current_status = PurePursuitStatus::Idle;
        
        /**
         * @brief Set the status of the manager.
         */
        void setStatus(int new_status);

        /**
         * @brief Traverses the provided path.
         * @param path The path to traverse.
         */
        void traversePath(Path path);

    public:

        /**
         * @brief Constructs a new Path Manager object.
         * @param properties A correctly-configured RobotProperties structure.
         */
        PathManager(RobotProperties properties);

        /**
         * @brief Gets the current status of the manager
         * @return A value from PurePursuitStatus - Idle or Traversing.
         */
        int getStatus();

        /**
         * @brief Gets the string representation of the manager's current status.
         */
        std::string getPrettyStatus();

        PathBuilder buildPath(std::string name, GenerationParameters g_params, QLength lookahead);

        void insertPath(Path path);

        //Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
        void pathfindLinear();

        //Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
        void pathfindDistance();

        /**
         * @brief Traverses a stored path.
         * @param path_name The name of the desired path.
         */
        void traverseStoredPath(std::string path_name);

        /**
         * @brief Dumps a stored path to std::cout.
         * @param path_name The name of the path to dump.
         */
        void dumpStoredPath(std::string path_name);

};