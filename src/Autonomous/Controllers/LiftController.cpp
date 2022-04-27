#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/CONTROLLERS.hpp"

QAngle LiftController::getLiftAngle()
{
    int pot_value = std::clamp(lift_potentiometer.get_value() - 1800, (int32_t)0, (int32_t)2295);
    return 140_deg - ((pot_value / POTENTIOMETER_SCALE_FACTOR) * degree); 
}

QAngle LiftController::getError()
{
    return -(getLiftAngle() - target);
}

void LiftController::engageController(QAngle target_angle)
{
    target = std::clamp(target_angle.convert(degree), 0.0, 140.0) * degree;
    is_settled = false;
}

void LiftController::bangLoop()
{
    QAngle error = getError();
    while(true)
    {
        if(!is_settled)
        {
            error = getError();
            lift_motor.move_voltage(12000 * sgnum(error.convert(degree)));
            if(error.abs() < 1_deg) { is_settled = true; lift_motor.move_voltage(0); }
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