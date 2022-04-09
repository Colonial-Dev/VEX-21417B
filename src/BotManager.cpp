#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/BotManager.hpp"

void BotManager::setBrakeMode(pros::motor_brake_mode_e_t brake_mode)
{
    right_back.set_brake_mode(brake_mode);
    right_middle.set_brake_mode(brake_mode);
    right_front.set_brake_mode(brake_mode);
    left_back.set_brake_mode(brake_mode);
    left_middle.set_brake_mode(brake_mode);
    left_front.set_brake_mode(brake_mode);

    arm_motor.set_brake_mode(brake_mode);
    conveyor_motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}


void BotManager::registerDriverTask(pros::task_t task)
{
    pros::Task newTask(task);
    newTask.notify();
    driverTasks.push_back(newTask);
}

void BotManager::suspendDriverControl()
{
    for(int i = 0; i < driverTasks.size(); i++)
    {
        driverTasks.at(i).notify_ext(0, NOTIFY_ACTION_OWRITE, NULL);
    }
}

void BotManager::resumeDriverControl()
{
    for(int i = 0; i < driverTasks.size(); i++)
    {
        driverTasks.at(i).notify();
    }
}

//Resets all relative sensor values to default, simulating a restart of the program.
//Causes a segmentation fault for some fucking reason???
void BotManager::reinitialize()
{
    inertial_sensor.reset();
    left_encoder.reset();
    middle_encoder.reset();
    right_encoder.reset();
    drive_train->getOdometry()->step();
    drive_train->setState({0_in, 0_in, 0_deg});
    imu_odometer.reset();
}

void autonomousAsync()
{
    overwatch.suspendDriverControl();
    pros::delay(50);
    master.rumble("-");
    pros::delay(1000);
    master.rumble("-");
    pros::delay(1000);
    master.rumble("---");
    pros::delay(1000);

    autonomous();

    overwatch.resumeDriverControl();
}