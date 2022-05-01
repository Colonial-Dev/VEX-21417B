#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/Controllers/LiftController.hpp"

QAngle LiftController::getLiftAngle()
{
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
    while(true)
    {
        if(!is_settled)
        {
            error = getError();
            //if(sgnum(error.convert(degree)) != sgnum(starting_error.convert(degree))) { is_settled = true; lift_motor.move_voltage(0); }
            if(error.abs() < 2_deg) { is_settled = true; lift_motor.move_voltage(0); }
            else { lift_motor.move_voltage(12000 * sgnum(error.convert(degree))); }
        }
        pros::delay(15);
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