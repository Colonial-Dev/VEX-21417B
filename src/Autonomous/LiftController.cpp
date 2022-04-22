#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/LiftController.hpp"

QAngle LiftController::getLiftAngle()
{
    int pot_value = std::clamp(lift_potentiometer.get_value() - 1800, (int32_t)0, (int32_t)2295);
    return 140_deg - ((pot_value / POTENTIOMETER_SCALE_FACTOR) * degree); 
}

QAngle LiftController::getError()
{
    return -(getLiftAngle() - target);
}

void LiftController::clampAndSetTarget(QAngle target_angle)
{
    target = std::clamp(target_angle.convert(degree), 0.0, 140.0) * degree;
}

void LiftController::bangLoop()
{
    QAngle error = getError();
    is_settled = false;
    while(error.abs() > 1_deg)
    {
        error = getError();
        lift_motor.move_voltage(12000 * sgnum(error.convert(degree)));
        pros::delay(20);
    }

    lift_motor.move_voltage(0);
    is_settled = true; 
}

LiftController::LiftController(pros::Motor& motor, pros::ADIAnalogIn& pot) : lift_motor(motor), lift_potentiometer(pot)
{

}

void LiftController::setTarget(QAngle target_angle)
{
    if(!is_settled) { return; }
    clampAndSetTarget(target_angle);
    bangLoop();
}

void LiftController::setTargetAsync(QAngle target_angle)
{
    if(!is_settled) { return; }
    clampAndSetTarget(target_angle);
    pros::Task bang([this] { bangLoop(); }, TASK_PRIORITY_DEFAULT);
}

void LiftController::waitUntilSettled()
{
    while(!is_settled)
    {
        pros::delay(10);
    }
}