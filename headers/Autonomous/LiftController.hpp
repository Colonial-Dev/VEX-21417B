#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"

//1800 top limit, 4095 bottom limit
//Transforming this to a more friendly reference frame, that's 0 for the top limit and 2295 for the bottom limit
//Another transformation gets our limits at 0 degrees for the top limit and 140 degrees for the bottom limit
//We can use (140 - desired angle) to flip the limits to 140 for the top and 0 for the bottom
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

        void clampAndSetTarget(QAngle target_angle);

        void bangLoop();

    public:

        LiftController(pros::Motor& motor, pros::ADIAnalogIn& pot);

        QAngle getLiftAngle();

        void setTarget(QAngle target_angle);

        void setTargetAsync(QAngle target_angle);

        void waitUntilSettled();
};