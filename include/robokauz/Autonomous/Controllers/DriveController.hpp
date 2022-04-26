#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

struct PIDConstants
{
    const double P_CONSTANT = 0.0;
    const double I_CONSTANT = 0.0;
    const double D_CONSTANT = 0.0;
};

struct ControllerTargets
{
    QAngle angle = 0_deg;
    QLength distance = 0_ft;
    Vector point = {0_ft, 0_ft};
};

enum ControllerState
{
    Settled,
    Turning,
    Translating
};

enum MotorMode
{
    Velocity,
    Voltage
};

enum TurnMode
{
    Absolute,
    Relative
};

class DriveController
{
    private:

        IMUOdometer& odometer;
        std::vector<pros::Motor> left_motors;
        std::vector<pros::Motor> right_motors;
        const PIDConstants TURN_CONSTANTS;
        const PIDConstants TRANSLATION_CONSTANTS;
        ControllerTargets controller_targets;
        int controller_state = Settled;

        QAngle getRobotHeading();

        QAngle getTurnError();

        void turnLoop();

    public:

        DriveController(std::vector<pros::Motor> left, std::vector<pros::Motor> right, PIDConstants turning_constants, PIDConstants translation_constants);

        void setLeftOutput(int output, int mode = Velocity);

        void setRightOutput(int output, int mode = Velocity);

        void setTankOutput(int left_output, int right_output, int mode = Velocity);

        void brake();

        void turnAngle(QAngle target, int mode);

        void turnToPoint(Point target);
};