#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"

struct EncoderGroup
{
    pros::ADIEncoder& left;
    pros::ADIEncoder& middle;
    pros::ADIEncoder& right;
};

struct SensorState
{
    QAngle rotation;
    int left;
    int middle;
    int right;
};

class InertialOdometer
{
    private:

        OdomState current_position;
        SensorState current_state;
        SensorState previous_state;

        pros::IMU& imu;
        pros::ADIEncoder& encoder_left;
        pros::ADIEncoder& encoder_middle;
        pros::ADIEncoder& encoder_right;

        QLength wheel_diameter;
        QLength wheel_circumfrence;

        QAngle getRotation();

        void updateState();

        void updatePosition(Vector displacement_vector, QAngle heading);

        void odometryLoop();

    public:

        InertialOdometer(pros::IMU& inertial, EncoderGroup& encoders, QLength tracking_wheel_diameter);

        OdomState getPosition();

};

