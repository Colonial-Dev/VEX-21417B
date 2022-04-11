#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/BotManager.hpp"

int BotManager::getConnectionMode()
{
    if(pros::competition::is_connected()) { return Manager::Switch; }
    else if(master.is_connected()) { return Manager::Wireless; }
    else { return Manager::None; }
}

int BotManager::getOperatingMode()
{
    if(!pros::competition::is_connected()) { return Manager::Free; }
    else if(pros::competition::is_disabled()) { return Manager::Disabled; }
    else if(pros::competition::is_autonomous()) { return Manager::Autonomous; }
    else { return Manager::Driver; }
}

std::string BotManager::getPrettyConnectionMode()
{
    switch(getConnectionMode())
    {
        case Manager::None:
        {
            return "[#808080 NONE#]";
        }
        case Manager::Switch:
        {
            return "[#00ff00 SWITCH#]";
        }
        case Manager::Wireless:
        {
            return "[#0000ff WIRELESS#]";
        }
    }
    return "[#ff0000 ERROR]";
}

std::string BotManager::getPrettyOperatingMode()
{
    switch(getOperatingMode())
    {
        case Manager::Disabled:
        {
            return "[#808080 DISABLED#]";
        }
        case Manager::Autonomous:
        {
            return "[#0000ff AUTON#]";
        }
        case Manager::Driver:
        {
            return "[#00ff00 DRIVER#]";
        }
        case Manager::Free:
        {
            return "[#00ffff FREE#]";
        }
    }
    return "[#ff0000 ERROR]";
}

std::string BotManager::getTemperatureColored(double temp)
{
    if(temp < 40.0) { return "#00ff00 " + precise_string(temp) + "#"; }
    else if(temp < 55.0) { return "#ffff00 " + precise_string(temp) + "#"; }
    else { return "#ff0000 " + precise_string(temp) + "#"; }
}

std::string BotManager::getPrettyTemperatures()
{
    double drive_train_temps = right_back.get_temperature() +
                               right_middle.get_temperature() +
                               right_front.get_temperature() +
                               left_back.get_temperature() +
                               left_middle.get_temperature() +
                               left_front.get_temperature();
    
    double manipulator_temps = arm_motor.get_temperature() +
                               conveyor_motor.get_temperature();

    drive_train_temps /= 6;
    manipulator_temps /= 2;

    std::string drive_train_avg = "DRIVE " + getTemperatureColored(drive_train_temps) + "C";
    std::string manipulator_avg = "MANIPS " + getTemperatureColored(manipulator_temps) + "C";
    return "[" + drive_train_avg + " | " + manipulator_avg + "]";
}

std::string BotManager::getPrettyOdomState()
{
    return "[" + 
           precise_string(drive_train->getState().x.convert(foot)) + 
           " | " + 
           precise_string(drive_train->getState().y.convert(foot)) +
           " | " + 
           precise_string(drive_train->getState().theta.convert(degree), 3) + 
           "]";
}

std::string BotManager::getPrettyBattery()
{
    return "[" + 
           std::to_string(int (pros::battery::get_capacity())) +
           "% | " + precise_string(pros::battery::get_voltage() / 1000.0, 3) 
           + "V]";
}

std::string BotManager::getPrettyEncoders()
{
    return "[L " +
            std::to_string(left_encoder.get_value()) +
            " | M " + std::to_string(middle_encoder.get_value()) +
            " | R " + std::to_string(right_encoder.get_value()) +
            "]";
}

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
    overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
}

void BotManager::resumeDriverControl()
{
    for(int i = 0; i < driverTasks.size(); i++)
    {
        driverTasks.at(i).notify();
    }
    overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

//Resets all relative sensor values to default, simulating a restart of the program.
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