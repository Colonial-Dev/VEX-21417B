#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/Vector2D.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/PathBuilder.hpp"
#include "robokauz/Autonomous/PurePursuit/PathTraverser.hpp"

/**
 * @brief Wrapper class for a speed change limiter using elapsed time between calls and maximum permissible acceleration as limits.
 */
class RateLimiter
{
    private:

        uint64_t last_called = pros::millis(); /** The last timestamp the limiter was called at. */

        QSpeed last_output; /** The last value output by the limiter. */
    
    public:

        /**
         * @brief Calculate the limited velocity from an input velocity and acceleration limit.
         * 
         * @param input The original, un-limited velocity.
         * @param limit The maximum permissible acceleration.
         */
        QSpeed getLimited(QSpeed input, QAcceleration limit);
};

struct WheelSpeeds
{
    QAngularSpeed target_left;
    QAngularSpeed target_right;
};

struct TraversalPoints
{
    OdomState current_position;
    PathPoint closest_point;
    Vector2D lookahead_point;
    int closest_index = 0;
    int lookahead_index = 0;
};

struct TraversalConditions
{
    QLength distance_to_end;
    bool is_on_path;
    bool end_within_lookahead;
    bool is_past_end;
    bool is_running = false;
    bool is_finished = false;
};

struct TraversalOutputs
{
    double curvature;
    WheelSpeeds target_speeds;
};

/**
 * @brief Semi-transient object used to mediate path traversal.
 */
class PathTraverser
{
    private:

        const RobotProperties robot_properties;
        RateLimiter limiter;
        Path path;

        TraversalPoints points;
        TraversalConditions conditions;
        TraversalOutputs outputs;

        void updatePosition();

        void findClosestPoint();

        double findIntersect(Vector2D start, Vector2D end, Vector2D pos, QLength lookahead);

        void calculateLookahead();

        void projectLookahead();

        void calculateCurvature();

        void calculateWheelSpeeds();

        void updateConditions();

        void calculateAll();

        bool integrateCalculations();

        bool checkIfJammed();

        void traversalLoop();

        void reset();

    public:

        PathTraverser(Path path_to_traverse, const RobotProperties robot);

        void traversePath();

        void traversePathAsync();

        bool isSettled();

        void waitUntilSettled();

        void forceAbort();
};