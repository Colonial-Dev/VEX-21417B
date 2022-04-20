#pragma once
#include "robokauz/PROS.hpp"

/**
 * @brief Constants and objects that model/control a robot and its components.
 */
struct RobotProperties
{   
    /**
     * @brief The maximum velocity reachable by the robot.
     * @note Can be lowered to cap the robot's speed.
     * @warning Raising this beyond the robot's actual maximum speed will cause impossible paths to be generated.
     */
    QSpeed max_velocity;

    /**
     * @brief The maximum acceleration achievable by the robot.
     * @note Can be lowered to cap the robot's acceleration.
     * @warning Raising this beyond the robot's actual maximum acceleration will cause impossible paths to be generated.
     */
    QAcceleration max_acceleration;

    /**
     * @brief The width between the drive wheels/tracks on the robot.
     * @warning Track width is measured between two different wheel tracks, typically on the left and right sides, NOT wheels on the same track/side.
     * @note Can be adjusted empirically to account for scrub and other factors.
     */
    QLength track_width;

    /**
     * @brief The diameter of the drive wheels.
     */
    QLength wheel_diam;

    //A shared_ptr to a correctly configured Okapi OdomChassisController.

    /**
     * @brief A shared_ptr to a correctly configured Okapi OdomChassisController. 
     * @details Used both for position tracking and robot movement.
     */
    std::shared_ptr<OdomChassisController> odom_controller;
};

/**
 * @brief Constants used in path generation - primarily in the smoothing operation.
 */
struct GenerationParameters
{
    /**
     * @brief Limits how fast the robot will take turns. The higher, the faster. Values between 1-5 work best in most situations.
     */
    double initial_velocity_constant;
    QLength lookahead_distance;
};

class Vector;

struct RawPoint
{
    QLength x_pos;
    QLength y_pos;

    RawPoint(Vector vector);
    RawPoint(Point point);
};

struct PathPoint
{
    QLength x_pos;
    QLength y_pos;
    QSpeed velocity;

    QLength distance;
    double curvature;
};