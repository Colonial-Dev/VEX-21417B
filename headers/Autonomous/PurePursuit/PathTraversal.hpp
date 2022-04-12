#pragma once
#include "robokauz/PROS.hpp"
#include "../VectorMath.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"

/**
 * @brief Target angular wheel speeds for the left and right sides. Used in path traversal.
 */
struct WheelSpeeds
{
    QAngularSpeed target_left;
    QAngularSpeed target_right;
};

/**
 * @brief Transient/single-use structure that holds working information during traversal of a path.
 */
struct TraversalCache
{

    /**
     * @brief A user-provided RobotProperties structure.
     */
    RobotProperties robot_properties;

    /**
     * @brief The robot's current position and rotation.
     */
    OdomState current_position;

    /**
     * @brief The path being followed.
     */
    Path path;

    /**
     * @brief The current closest point in the path.
     */
    PathPoint closest_point;

    /**
     * @brief The current Vector lookahead point.
     */
    Vector lookahead_point;

    /**
     * @brief The index of the current closest point in the path.
     */
    int closest_index = 0;

    /**
     * @brief The closest index in the path to the lookahead point.
     */
    int lookahead_index = 0;
    
    /**
     * @brief The current target wheel speeds.
     */
    WheelSpeeds target_speeds;
};

/**
 * @brief Update the robot's current position and rotation.
 * @param cache A reference to the TraversalCache structure to update.
 */
void updatePosition(TraversalCache& cache);

/**
 * @brief Update the closest point to the robot's current position.
 * @param cache A reference to the TraversalCache structure to update.
 */
void updateClosestPoint(TraversalCache& cache);

/**
 * @brief Find the t-value intersection between a path segment and the robot's position/lookahead distance.
 * 
 * @param start The start of the path segment.
 * @param end The end of the path segment.
 * @param pos The robot's current position. 
 * @param lookahead The lookahead distance, in meters.
 * @return The t_value intersection. If less than zero, no intersection was found.
 */
double findIntersect(Vector& start, Vector& end, Vector& pos, double lookahead);

/**
 * @brief Update the current lookahead point.
 * @param cache A reference to the TraversalCache structure to update.
 */
void updateLookaheadPoint(TraversalCache& cache);

/**
 * @brief Calculate the curvature from the robot's current position and angle to the lookahead point.
 * @param cache A reference to the TraversalCache structure to use.
 * @return double The calculated curvature.
 */
double calculateCurvature(TraversalCache& cache);

/**
 * @brief Checks to see if the robot is within a specified distance of the path's endpoint.
 * @param cache A reference to the TraversalCache structure to use.
 * @param threshold The desired distance threshold.
 * @return true The robot is within the threshold of the endpoint.
 * @return false The robot is not within the threshold of the endpoint.
 */
bool checkDistance(TraversalCache& cache, QLength threshold);

/**
 * @brief Calculate the wheel speeds needed to pursue the current lookahead point.
 * @param cache A reference to the TraversalCache structure to update.
 * @param curvature The curvature from the robot to the lookahead point.
 */
void calculateWheelSpeeds(TraversalCache &cache, double curvature);