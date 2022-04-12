#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"
#include "PathDumping.hpp"
#include "PathGeneration.hpp"
#include "PathTraversal.hpp"
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
         * @param parameters A configured TraversalParameters structure.
         * @param robot A correctly-configured RobotProperties structure.
         */
        PathTraverser(Path path_to_traverse, TraversalParameters parameters, RobotProperties robot);

        /**
         * @brief Traverses the path specified in the constructor.
         */
        void traversePath();

        /**
         * @brief Simulates the outputs from traversal algorithms given a hypothetical position, then dumps the results to std::cout.
         * @param position An OdomState structure set to the desired hypothetical position.
         */
        void simulatePath(OdomState position);
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
        void traversePath(Path path, TraversalParameters parameters);

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

        /**
         * @brief Generates a standard path and stores it in the PathManager instance.
         * @param path_name The name of the path. Must be unique.
         * @param parameters A configured GenerationParameters struct.
         * @param path_points A std::vector of Vectors representing the path outline.
         * Using brace initialization syntax is recommended, e.g.: {{0_ft, 0_ft}, {2_ft, 0_ft}}.
         */
        void generateStandardPath(std::string path_name, GenerationParameters parameters, std::vector<Vector> path_points);

        //Generates a temporary straight-line path from the robot's current position to a given point, then traverses it.
        void pathfindLinear();

        //Generates a temporary straight-line path from the robot's current position to a point X distance forwards, then traverses it.
        void pathfindDistance();

        /**
         * @brief Traverses a stored path.
         * @param path_name The name of the desired path.
         * @param parameters A configured TraversalParameters object.
         */
        void traverseStoredPath(std::string path_name, TraversalParameters parameters);

        /**
         * @brief Dumps a stored path to std::cout.
         * @param path_name The name of the path to dump.
         */
        void dumpStoredPath(std::string path_name);

};