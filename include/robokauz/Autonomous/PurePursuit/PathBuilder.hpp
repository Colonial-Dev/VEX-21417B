#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

/**
 * @brief Class that implements the builder pattern for composing new Pure Pursuit paths.
 * @note This class is not intended to be constructed manually, but instead should be acquired indirectly via a PathManager instance.
 */
class PathBuilder
{
    private:
        
        std::string path_name; /** The name of the new path. */

        GenerationParameters gen_params; /** The generation parameters to use when creating the path. */

        RobotProperties robot_props; /** The robot properties to use when creating the path. */

        std::vector<Waypoint> path_waypoints; /** The (X,Y) waypoints that outline the path. */

        PathManager& calling_manager; /** The PathManager that instantiated this instance. */

        bool is_reversed = false; /** Whether or not the new path is reversed. */

        bool compute_reversed = false; /** Whether or not to compute a reversed copy of the new path during generation. */

        bool do_debug_dump = false; /** Whether or not to print a debug dump of the new path once it has been generated. */

        /**
         * @brief Calculate the curvature of each point in the new path.
         * @note Uses the Menger Curvature method: https://en.wikipedia.org/wiki/Menger_curvature
         * @param path A reference to the path to calculate curvatures for.
         */
        void calculateCurvatures(Path& path);
        
        /**
         * @brief Calculate the ideal velocity at each point along the new path using simple kinematics.
         * @param path A reference to the path to calculate velocities for.
         */
        void calculateVelocities(Path& path);

        /**
         * @brief Generate a new path according to known information and constraints.
         */
        Path calculatePath();

    public:
        
        /**
         * @brief Construct a new PathBuilder object.
         * @note Should not be constructed directly, but instead acquired indirectly via a PathManager instance.
         * @param name The name of the new path.
         * @param g_params The GenerationParameters to use.
         * @param r_props The RobotProperties to use.
         * @param caller The PathManager responsible for constructing the instance.
         */
        PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller);

        /**
         * @brief Specify an alternate set of robot properties to use.
         * @param r_props The desired robot properties.
         */
        PathBuilder withRobotProperties(RobotProperties r_props);

        /**
         * @brief Register an outline waypoint.
         * @param point The waypoint to register.
         */
        PathBuilder withPoint(Waypoint point);

        /**
         * @brief Register a set of outline waypoints.
         * @param point The waypoints to register.
         */
        PathBuilder withPoints(std::vector<Waypoint> points);

        /**
         * @brief Register the origin - (0, 0) - as an outline waypoint.
         */
        PathBuilder withOrigin();

        /**
         * @brief Register the current position from an IMUOdometer instance as an outline waypoint.
         * @param odometer The IMUOdometer instance to get the current position from.
         */
        PathBuilder withCurrentPosition(IMUOdometer& odometer);

        /**
         * @brief Register whether or not the path is reversed.
         */
        PathBuilder isReversed(bool val = true);
        
        /**
         * @brief Flag that the PathBuilder should generate a reversed copy of the new path.
         */
        PathBuilder makeReversed();

        /**
         * @brief Flag that the PathBuilder should perform a debug dump of the generated path.
         */
        PathBuilder withDebugDump();

        /**
         * @brief Finalize all parameters and generate the new path.
         */
        void generatePath();
};
