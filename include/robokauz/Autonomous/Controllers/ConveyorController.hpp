#pragma once
#include "robokauz/PRELUDE.hpp"

enum ConveyorStatus
{
  Idle,
  Forward,
  Reverse
};

/**
 * @brief Wrapper class for controlling a ringle conveyor belt. Uses a simple finite state machine to enable toggle control.
 */
class ConveyorController
{
    private:

        /**
         * @brief A reference to the conveyor motor, passed in during construction.
         */
        pros::Motor& conveyor_motor;

        /**
         * @brief The set state of the controller.
         */
        int conveyor_state = Idle;

        /**
         * @brief Enable/disable flag for automatic conveyor unjamming.
         */
        bool unjam_enabled = true;

        /**
         * @brief Task loop that runs the controller. Launched during construction.
         */
        void controlLoop();

    public:

        /**
         * @brief Construct a new ConveyorController object.
         * @param motor The motor running the conveyor belt.
         */
        ConveyorController(pros::Motor& motor);

        /**
         * @brief Gets the current state of the controller.
         * @return An integer equating to a value of ConveyorStatus (Idle, Forward, Reverse).
         */
        int getState();
        
        /**
         * @brief Sets the desired state of the controller.
         * @param new_state The desired state as a value of ConveyorStatus (Idle, Forward, Reverse).
         */
        void setState(int new_state);

        /**
         * @brief Driver control hook. When called, if the controller state is Idle or Reverse, it will be
         * set to Forward. If it is already Forward, it will be set to Idle.
         */
        void toggleForwards();

        /**
         * @brief Driver control hook. When called, if the controller state is Idle or Forwards, it will be
         * set to Reverse. If it is already Reverse, it will be set to Idle.
         */
        void toggleReverse();

        /**
         * @brief Enables automatic unjamming.
         */
        void enableUnjam();

        /**
         * @brief Disables automatic unjamming.
         */
        void disableUnjam();

};