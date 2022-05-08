#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>
#include <optional>

namespace PurePursuitStatus
{
    enum PurePursuitStatus
    {
        Idle,
        Traversing
    };
}

/**
 * @brief Abstraction class for managing generated paths and using the pure pursuit controller.
 */
class PathManager
{
    private:

        RobotProperties robot_props; /** The default properties of the robot. */

        std::map<std::string, Path> stored_paths; /** Associative map storing generated paths correlated by their string names. */

        std::optional<PathTraverser> current_traverser; /** The PathTraverser currently in use, if any. */

    public:

        /**
         * @brief Construct a new PathManager object.
         * @param properties The default properties of the robot.
         */
        PathManager(RobotProperties properties);

        /**
         * @brief Get the current status of the internal PathTraverser, if any.
         * @return A value of PurePursuitStatus::PurePursuitStatus (Idle or Traversing).
         */
        int getStatus();

        /**
         * @brief Get a pretty string representation of the controller's current status.
         */
        std::string getPrettyStatus();

        /**
         * @brief Construct a PathBuilder instance for new path creation.
         * @param name The name of the new path. Should be unique; paths with a duplicate name will overwrite any previously generated paths of the same name.
         * @param g_params The generation parameters to use for the new path.
         */
        PathBuilder buildPath(std::string name, GenerationParameters g_params);

        /**
         * @brief Insert a path into the manager's internal associative map.
         * @param path The path to insert. The key used to store it internally is the path's name; if a particular name/key already exists, it will be overwritten.
         */
        void insertPath(Path path);

        /**
         * @brief Delete a path with the specified name.
         * @param path_name The name of the path to delete.
         */
        void deletePath(std::string path_name);

        /**
         * @brief Traverse a path of the specified name. Blocks until traversal is finished.
         * @param path_name The name of the path to traverse.
         */
        void synchronousTraverse(std::string path_name);

        /**
         * @brief Traverse a path of the specified name. Does not block until traversal is finished, and instead continues immediately.
         * @param path_name The name of the path to traverse.
         */
        void asynchronousTraverse(std::string path_name);

        /**
         * @brief Generate a transient path between the robot's current position and the specified point, then traverse it synchronously.
         * @param point The point to travel to.
         * @param backwards Whether or not to move backwards.
         */
        void traverseLinear(Point point, bool backwards = false);

        /**
         * @brief Generate a transient path between the robot's current position and the specified point, then traverse it asynchronously.
         * @param point The point to travel to.
         * @param backwards Whether or not to move backwards.
         */
        void traverseLinearAsync(Point point, bool backwards = false);

        /**
         * @brief Synchronously traverses the specified distance forwards or backwards irrespective of the robot's absolute position.
         * @param distance The distance to traverse. (Negative will result in backwards motion.)
         */
        void traverseDistance(QLength distance);
        
        /**
         * @brief Asynchronously traverses the specified distance forwards or backwards irrespective of the robot's absolute position.
         * @param distance The distance to traverse. (Negative will result in backwards motion.)
         */
        void traverseDistanceAsync(QLength distance);

        /**
         * @brief Block until the current PathTraverser - if any - completes traversal.
         */
        void waitUntilSettled();

        /**
         * @brief Forcefully abort any ongoing traversal.
         */
        void forceAbort();

        /**
         * @brief Get a raw PathTraverser object based on a path of the specified name. Intended for debug use only.
         * @param path_name The name of the path to construct the Traverser with.
         */
        PathTraverser getTraverser(std::string path_name);

        /**
         * @brief Debug dump a path of the specified name.
         * @param path_name The name of the path to debug dump.
         */
        void dumpPath(std::string path_name);
};