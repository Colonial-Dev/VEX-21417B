#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

/**
 * @brief Holds references to the left, middle and right tracking wheel ADI encoders.
 */
struct EncoderGroup
{
    pros::ADIEncoder& left;
    pros::ADIEncoder& middle;
    pros::ADIEncoder& right;
};

/**
 * @brief Holds a snapshot of the robot's rotation and tracking encoder positions.
 */
struct SensorState
{
    QAngle rotation = 0_deg;
    int left = 0;
    int middle = 0;
    int right = 0;
};

/**
 * @brief Represents the state of an IMUOdometer instance.
 */
enum IMUOdomState
{
    Error, /** Inertial sensor not found. */
    Calibrating, /** Inertial sensor is calibrating. */
    Operational /** Inertial sensor is OK; position tracking is online. */
};

class IMUOdometer
{
    private:

        /**
         * @brief Holds the robot's current position.
         */
        OdomState current_position;

        /**
         * @brief Holds the robot's current SensorState.
         */
        SensorState current_state;

        /**
         * @brief Holds the robot's previous SensorState.
         */
        SensorState previous_state;

        /**
         * @brief Holds a reference to the IMU.
         */
        pros::IMU& imu;

        pros::ADIEncoder& encoder_left; /** Holds a reference to the left ADI encoder. */
        pros::ADIEncoder& encoder_middle; /** Holds a reference to the middle ADI encoder. */
        pros::ADIEncoder& encoder_right; /** Holds a reference to the right ADI encoder. */

        QLength wheel_circumfrence; /** The robot's wheel circumfrence. */

        /**
         * @brief Gets the rotation value output by the IMU.
         */
        QAngle getRotation();

        /**
         * @brief Updates the robot's previous and current SensorStates.
         */
        void updateState();

        /**
         * @brief Updates the robot's current position. 
         * @param displacement_vector The displacement vector from the previous position.
         * @param heading The robot's current rotation.
         */
        void updatePosition(Vector displacement_vector, QAngle heading);

        /**
         * @brief Task loop - updates the robot's position every ~10 milliseconds.
         */
        void odometryLoop();

    public:
        
        /**
         * @brief Constructs a new IMUOdometer object.
         * 
         * @param inertial A reference to an IMU object.
         * @param encoders A reference to an EncoderGroup structure.
         * @param tracking_wheel_diameter The diameter of the tracking wheels.
         */
        IMUOdometer(pros::IMU& inertial, EncoderGroup& encoders, QLength tracking_wheel_diameter);

        /**
         * @brief Gets the robot's current position and rotation.
         */
        OdomState getPosition();

        /**
         * @brief Sets the robot's current position and rotation.
         * @note Best used to establish field-centric rather than start-centric coordinates.
         * @param position An OdomState structure representing the desired new position and rotation.
         */
        void setPosition(OdomState position);

        /**
         * @brief Reset the IMUOdometer to zero position, zero rotation.
         */
        void reset();

        /**
         * @brief Gets a prettified string representation of the robot's current position and rotation.
         */
        std::string getPrettyPosition();

        /**
         * @brief Get the status of the IMUOdometer
         * @return A value from IMUOdomState - Error, Calibrating or Operational. 
         */
        int getStatus();

};

