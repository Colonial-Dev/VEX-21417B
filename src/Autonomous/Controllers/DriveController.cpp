#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/Controllers/DriveController.hpp"

DriveController::DriveController(std::vector<int> left, std::vector<int> right, IMUOdometer& imu_odom, PIDConstants turning_constants) : odometer(imu_odom), TURN_CONSTANTS(turning_constants)
{   
    for(int port : left)
    {
        bool reversed = (sgnum(port) == 1) ? false : true;
        left_motors.push_back(pros::Motor(port, reversed));
    }

    for(int port : right)
    {
        bool reversed = (sgnum(port) == 1) ? false : true;
        right_motors.push_back(pros::Motor(port, reversed));
    }

    pros::Task turningLoop([this] { turnLoop(); }, TASK_PRIORITY_DEFAULT + 1);
}

QAngle DriveController::getRobotHeading()
{
    return (odometer.getPosition().theta); 
}

double DriveController::getError()
{
    return target_angle.convert(degree) - getRobotHeading().convert(degree);
}

void DriveController::resetOutputs()
{
    PIDOutputs new_outputs;
    outputs = new_outputs;
}

void DriveController::turnLoop()
{   
    while(true)
    {
        if(!turn_settled)
        {
            outputs.error = getError();
            outputs.integral = (std::fabs(outputs.error) <= 15.0) ? (outputs.integral + outputs.error) : outputs.integral;
            outputs.integral = (outputs.error == 0 || sgnum(outputs.error) != sgnum(outputs.prev_error)) ? 0 : outputs.integral;
            outputs.derivative = outputs.error - outputs.prev_error;
            outputs.prev_error = outputs.error;

            if(std::fabs(outputs.error) < 4.0 && std::fabs(outputs.derivative) < 4.0) { outputs.threshold_ct++; }
            else { outputs.threshold_ct = 0; }

            if(outputs.threshold_ct >= 10) { turn_settled = true; resetOutputs(); brake(); continue; }

            double vel = (outputs.error * TURN_CONSTANTS.P_CONSTANT) + (outputs.integral * TURN_CONSTANTS.I_CONSTANT) + (outputs.derivative * TURN_CONSTANTS.D_CONSTANT);

            tank(vel, -vel, Velocity);
        }
        pros::delay(10);
    }
}

void DriveController::left(int output, int mode)
{
    switch(mode)
    {
        case Velocity:
        {
            for(pros::Motor motor : left_motors)
            {
                motor.move_velocity(output);
            }
        }
        case Voltage:
        {
            for(pros::Motor motor : left_motors)
            {
                motor.move_voltage(output);
            }  
        }
    }
}

void DriveController::right(int output, int mode)
{
    switch(mode)
    {
        case Velocity:
        {
            for(pros::Motor motor : right_motors)
            {
                motor.move_velocity(output);
            }
        }
        case Voltage:
        {
            for(pros::Motor motor : right_motors)
            {
                motor.move_voltage(output);
            }  
        }
    }
}

void DriveController::tank(int left_output, int right_output, int mode)
{
    left(left_output, mode);
    right(right_output, mode);
}

void DriveController::brake()
{
    left(0);
    right(0);
}

void DriveController::turnAbsolute(QAngle target)
{
    target_angle = target;
    turn_settled = false;
}

void DriveController::turnRelative(QAngle target)
{

}

void DriveController::turnToPoint(Point point)
{

}

void DriveController::waitUntilSettled()
{
    while(!turn_settled)
    {
        pros::delay(10);
    }
}