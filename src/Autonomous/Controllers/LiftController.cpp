#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/Controllers/LiftController.hpp"

QAngle LiftController::getLiftAngle()
{
    //1800 top limit, 4095 bottom limit
    //Transforming this to a more friendly reference frame, that's 0 for the top limit and 2295 for the bottom limit
    //Another transformation gets our limits at 0 degrees for the top limit and 140 degrees for the bottom limit
    //We can use (140 - desired angle) to flip the limits to 140 for the top and 0 for the bottom
    
    int pot_value = std::clamp(lift_potentiometer.get_value() - 1800, (int32_t)0, (int32_t)2295);
    QAngle angle = 140_deg - ((pot_value / POTENTIOMETER_SCALE_FACTOR) * degree);
    return std::clamp(angle, 0_deg, 140_deg);
}

QAngle LiftController::getError()
{
    return std::clamp(-(getLiftAngle() - target), -140_deg, 140_deg);
}

void LiftController::engageController(QAngle target_angle)
{
    target = std::clamp(target_angle.convert(degree), 0.0, 140.0) * degree;
    is_settled = false;
}

void LiftController::bangLoop()
{
    QAngle error = getError();
    QAngle starting_error = error;
    int jam_threshold_ct = 0;
    while(true)
    {
        if(!is_settled)
        {
            error = getError();

            if(lift_motor.get_efficiency() <= 0.1) { jam_threshold_ct++; }
            else { jam_threshold_ct = 0; }

            if(jam_threshold_ct >= 20) { is_settled = true; jam_threshold_ct = 0; lift_motor.move_velocity(0); continue; }

            if(error.abs() < 2_deg) { is_settled = true; lift_motor.move_velocity(0); }
            else { lift_motor.move_voltage(MOTOR_MAX_VOLTAGE * sgnum(error.convert(degree))); }
        }
        pros::delay(10);
    }
}

LiftController::LiftController(pros::Motor& motor, pros::ADIAnalogIn& pot) : lift_motor(motor), lift_potentiometer(pot)
{
    pros::Task bang([this] { bangLoop(); }, TASK_PRIORITY_DEFAULT);
}

void LiftController::setTarget(QAngle target_angle)
{
    engageController(target_angle);
    waitUntilSettled();
}

void LiftController::setTargetAsync(QAngle target_angle)
{
    engageController(target_angle);
}

void LiftController::waitUntilSettled()
{
    while(!is_settled)
    {
        pros::delay(10);
    }
}