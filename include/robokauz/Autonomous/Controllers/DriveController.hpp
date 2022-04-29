#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

enum MotorMode
{
    Velocity,
    Voltage
};

struct PIDConstants
{
    const double P_CONSTANT = 0.0;
    const double I_CONSTANT = 0.0;
    const double D_CONSTANT = 0.0;
};

struct PIDOutputs
{
    double error = 0;
    double prev_error = 0;
    double proportional = 0;
    double integral = 0;
    double derivative = 0;
    double threshold_ct = 0;
};

class DriveController
{
    private:

        IMUOdometer& odometer;
        const PIDConstants TURN_CONSTANTS;
        PIDOutputs outputs;
        QAngle target_angle = 0_deg;
        bool turn_settled = true;

        std::vector<pros::Motor> left_motors;
        std::vector<pros::Motor> right_motors;

        QAngle getRobotHeading();

        double getError();

        void resetOutputs();

        void turnLoop();

    public:

        DriveController(std::vector<int> left, std::vector<int> right, IMUOdometer& imu_odom, PIDConstants turning_constants);

        void left(int output, int mode = Velocity);

        void right(int output, int mode = Velocity);

        void tank(int left_output, int right_output, int mode = Velocity);

        void brake();

        void turnAbsolute(QAngle target);

        void turnRelative(QAngle target);

        void turnToPoint(Point point);

        void waitUntilSettled();
};