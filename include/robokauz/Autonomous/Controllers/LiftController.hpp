#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

class LiftController
{
    private:

        //Potentiometer readout / SCALE_FACTOR = degrees equivalent
        //Degrees equivalent * SCALE_FACTOR = potentiometer equivalent
        const double POTENTIOMETER_SCALE_FACTOR = 16.38;

        pros::Motor& lift_motor;

        pros::ADIAnalogIn& lift_potentiometer;

        QAngle target = 0_deg;

        bool is_settled = true;

        QAngle getError();

        void engageController(QAngle target_angle);

        void bangLoop();

    public:

        LiftController(pros::Motor& motor, pros::ADIAnalogIn& pot);

        QAngle getLiftAngle();

        void setTarget(QAngle target_angle);

        void setTargetAsync(QAngle target_angle);

        void waitUntilSettled();
};