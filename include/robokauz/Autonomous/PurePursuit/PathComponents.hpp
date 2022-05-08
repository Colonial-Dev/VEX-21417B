#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/Autonomous/Controllers/DriveController.hpp"

/**
 * @brief Data about the robot's physical properties and capabilities, as well as a ptr to a DriveController.
 */
struct RobotProperties
{   
    QSpeed min_velocity; /** The minimum velocity of the robot during pure pursuit. Must be greater than zero. */
    QSpeed max_velocity; /** The maximum velocity of the robot during pure pursuit. Should not exceed the robot's actual capabilities. */
    QAcceleration max_acceleration; /** The maximum acceleration of the robot during pure pursuit. Should not exceed the robot's actual capabilities. */
    QLength track_width; /** The track width of the robot's drive train. */
    QLength wheel_diam; /** The diameter of the robot's drive wheels. */
    std::shared_ptr<DriveController> controller; /** A shared_ptr to a DriveController instance, for pure pursuit output. */
};

/**
 * @brief Parameters relevant to the generation of a pure pursuit path.
 */
struct GenerationParameters
{   
    /**
     * @brief The initial velocity constant, or k-value, for a path. Higher means faster turns/corners during pure pursuit.
     * @note Values between 1 and 5 seem to work best.
     */
    double initial_velocity_constant;

    /**
     * @brief The lookahead distance to assign to a path.
     * @note Smaller values theoretically mean closer adherence to the path, but in practice lead to oscillations.
     * Larger values result in smoother behavior, but more "corner cutting." Values of 16-18 inches seemed to work best for most cases.
     */
    QLength lookahead_distance;

    int spline_resolution = 30; /** The number of points to interpolate between each waypoint in a path. */
    double smoothing_constant = 1.0; /** How smooth/curvy a path should be. 0 means no curvature, 2 means a lot. Values of 0.5 to 1.0 seem to work the best. */
};

/**
 * @brief Represents an "outline" waypoint on a pure pursuit path.
 */
struct Waypoint
{
    QLength x_pos; /** The X-coordinate of the waypoint. */
    QLength y_pos; /** The Y-coordinate of the waypoint. */
    QAngle heading = 0_deg; /** The heading of the waypoint. */
};

/**
 * @brief Transient representation of a point in a pure pursuit path, used during quintic spline interpolation.
 */
struct SplinePoint
{
    QLength x_pos; /** The X-coordinate of the point. */
    QLength y_pos; /** The Y-coordinate of the point. */
    QAngle heading; /** The heading of the point. */
    QSpeed velocity; /** The velocity at the point. */
};

/**
 * @brief Final representation of a point in a pure pursuit path, used during traversal.
 */
struct PathPoint
{
    QLength x_pos; /** The X-coordinate of the point. */
    QLength y_pos; /** The Y-coordinate of the point. */
    QSpeed velocity = 0_mps; /** The velocity at the point. */
    double curvature = 0; /** The curvature at the point. */
};

/**
 * @brief Represents a ready-to-traverse Pure Pursuit path.
 */
class Path
{
    public:

        std::string name; /** The name of the path. */

        std::vector<PathPoint> points; /** The points that make up the path. */

        QLength lookahead_distance; /** The lookahead distance assigned to the path during generation. */

        double k_constant; /** The initial velocity or K-constant assigned to the path during generation. */

        bool reversed = false; /** Whether or not the path is reversed. */
        
        /**
         * @brief Gets the number of points in the path.
         */
        int size();

        /**
         * @brief Gets a reference to the point in the path at a specified index.
         * @param index The index of the desired point.
         */
        PathPoint& at(int index);

        /**
         * @brief Gets a reference to the point at the start of the path.
         */
        PathPoint& start();

        /**
         * @brief Gets a reference to the point at the end of the path.
         */
        PathPoint& end();

        /**
         * @brief Appends a point to the end of the path.
         * @param point The point to append.
         */
        void add(PathPoint point);
};