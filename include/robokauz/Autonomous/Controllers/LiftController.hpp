#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

/**
 * @brief Wrapper class for controlling a single-motor lift. Uses a potentiometer in an augmented bang-bang controller.
 */
class LiftController
{
    private:

        /**
         * @brief Constant scale factor between the raw ADI readout of the potentiometer and an equivalent degree measurement.
         * @note Potentiometer readout / SCALE_FACTOR = degrees equivalent
         * @note Degrees equivalent * SCALE_FACTOR = potentiometer equivalent
         */
        const double POTENTIOMETER_SCALE_FACTOR = 16.38;
        
        /**
         * @brief A reference to the lift motor, passed in during construction.
         */
        pros::Motor& lift_motor;

        /**
         * @brief A reference to the attached potentiometer, passed in during construction.
         */
        pros::ADIAnalogIn& lift_potentiometer;

        /**
         * @brief The current target of the bang-bang controller.
         */
        QAngle target = 0_deg;

        /**
         * @brief Whether or not the bang-bang controller is settled. Starts as true to prevent unexpected movement.
         */
        bool is_settled = true;
        
        /**
         * @brief Get the angular error between the target angle and measured lift angle.
         */
        QAngle getError();
        
        /**
         * @brief Set the desired target angle and mark the controller as not settled, engaging it.
         * @param target_angle The desired target angle, from 0 to 140 degrees.
         */
        void engageController(QAngle target_angle);

        /**
         * @brief Task loop that runs the controller. Launched during construction.
         */
        void bangLoop();

    public:

        /**
         * @brief Construct a new LiftController object.
         * @param motor The motor powering the lift.
         * @param pot The potentiometer attached to the lift.
         */
        LiftController(pros::Motor& motor, pros::ADIAnalogIn& pot);

        /**
         * @brief Gets the current angle of the lift.
         * @return QAngle The current angle of the lift, from 0 to 140 degrees.
         */
        QAngle getLiftAngle();

        /**
         * @brief Engages the controller with the desired target angle, then block until settled.
         * @param target_angle The target angle.
         */
        void setTarget(QAngle target_angle);

        /**
         * @brief Engages the controller with the desired target angle, then continue without waiting for it to settle.
         * @param target_angle The target angle.
         */
        void setTargetAsync(QAngle target_angle);
        
        /**
         * @brief Blocks until the controller has settled.
         */
        void waitUntilSettled();
};