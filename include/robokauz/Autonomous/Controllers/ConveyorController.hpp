#pragma once
#include "robokauz/PRELUDE.hpp"

enum ConveyorStatus
{
  Idle,
  Forward,
  Reverse
};

class ConveyorController
{
    private:

        pros::Motor& conveyor_motor;

        int conveyor_state = Idle;

        bool unjam_enabled = true;

        void controlLoop();

    public:

        ConveyorController(pros::Motor& motor);

        int getState();

        void setState(int new_state);

        void toggleForwards();

        void toggleReverse();

        void enableUnjam();

        void disableUnjam();

};