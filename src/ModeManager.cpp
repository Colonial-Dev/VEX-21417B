#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/ModeManager.hpp"


void ModeManager::registerDriverTask(pros::task_t task)
{
    pros::Task newTask(task);
    newTask.notify();
    driverTasks.push_back(newTask);
}

void ModeManager::suspendDriverControl()
{
    for(int i = 0; i < driverTasks.size(); i++)
    {
        driverTasks.at(i).notify_ext(0, NOTIFY_ACTION_OWRITE, NULL);
    }
}

void ModeManager::resumeDriverControl()
{
    for(int i = 0; i < driverTasks.size(); i++)
    {
        driverTasks.at(i).notify();
    }
}

//Resets all relative sensor values to default, simulating a restart of the program.
//Causes a segmentation fault for some fucking reason???
void ModeManager::reinitialize()
{
    inertial_sensor.reset();
    left_encoder.reset();
    middle_encoder.reset();
    right_encoder.reset();
    drive_train.reset();
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