#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/Controllers/ConveyorController.hpp"

void ConveyorController::setState(int new_state) { conveyor_state = new_state; }

int ConveyorController::getState() { return conveyor_state; }

void ConveyorController::enableUnjam() { unjam_enabled = true; }

void ConveyorController::disableUnjam() { unjam_enabled = false; }

ConveyorController::ConveyorController(pros::Motor& motor) : conveyor_motor(motor) 
{
     /*Everything else is default-initialized*/
    pros::Task conveyor([this] { controlLoop(); }, TASK_PRIORITY_DEFAULT); 
}

void ConveyorController::controlLoop()
{
    int jam_threshold_ct = 0;
    while(true)
    {
        switch(getState())
        {
            case Idle:
            {
                conveyor_motor.move_voltage(0);
                break;
            }
            case Forward:
            {
                conveyor_motor.move_voltage(-MOTOR_MAX_VOLTAGE);
                break;
            }
            case Reverse:
            {
                conveyor_motor.move_voltage(MOTOR_MAX_VOLTAGE);
                break;
            }
        }
        
        //PRINT(std::to_string(conveyor_motor.get_efficiency()));
        if(conveyor_motor.get_efficiency() <= 10 && getState() != Idle) { jam_threshold_ct++; }
        else { jam_threshold_ct = 0; }

        if(jam_threshold_ct >= 50 && unjam_enabled)
        {   
            conveyor_motor.move_voltage(MOTOR_MAX_VOLTAGE);
            pros::delay(5000);
            continue;
        }

        pros::delay(10);
    }
}

void ConveyorController::toggleForwards()
{
    if(getState() == Idle || getState() == Reverse) { setState(Forward); }
    else { setState(Idle); }
} 

void ConveyorController::toggleReverse()
{
    if(getState() == Idle || getState() == Forward) { setState(Reverse); }
    else { setState(Idle); }
}