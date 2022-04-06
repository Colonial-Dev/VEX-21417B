#pragma once

//Constants and objects that model the robot and its components.
//Max velocity and max acceleration can be set to any value within the actual capabilities of the robot.
//Track width is the distance between the drive wheels on the robot. May be adjusted empirically to account for scrub.
//Wheel diameter is the diameter of the drive wheels.
//odom_controller is a shared_ptr to a correctly configured Okapi OdomChassisController.
//kv, ka and kp are PID constants and should be adjusted as needed.
struct RobotProperties
{
    QSpeed max_velocity;
    QAcceleration max_acceleration;
    QLength track_width;
    QLength wheel_diam;
    std::shared_ptr<OdomChassisController> odom_controller;
    double kv;
    double ka;
    double kp;
};

//Constants used in path generation - primarily in the smoothing operation.
//A larger smooth_weight means a smoother path. Values of 0.75-0.98 seem to work well.
//data_weight should be equal to (1 - smooth_weight). 
//Tolerance is best held constant at 0.001.
//intial_velocity_constant mainly controls how fast the bot takes turns.
struct GenerationParameters
{
    double data_weight;
    double smooth_weight;
    double tolerance;
    double initial_velocity_constant;
};

struct PathPoint
{
    QLength x_pos;
    QLength y_pos;
    QLength distance;
    QSpeed target_velocity;
    double curvature;
};