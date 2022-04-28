#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

namespace Manager
{
    enum ConnectionMode
    {
        None, //Robot is not connected to any external devices.
        Switch, //Robot is connected to competition control (and implicitly, a controller.)
        Wireless //Robot is connected to a controller but not competition control.
    };

    enum OperatingMode
    {
        Disabled, //Robot has been disabled by a comp switch
        Autonomous, //Robot has been set to autonomous mode by a comp switch. Note: this blocks communication from AND to the controller.
        Driver, //Robot has been set to driver mode by a comp switch.
        Free //Robot is running unswitched and all functionality is accessible.
    };
}

class BotManager
{
    private:

        /**
         * @brief Gets the color-coded string version of a motor temperature value.
         */
        std::string getTemperatureColored(double temp);

        std::string getBatteryColored(int level);

        std::string getVoltageColored(double level);

        std::string getCurrentColored(double level);

    public:

        /**
         * @brief Gets the robot's current connection mode.
         * @return A value of Manager::ConnectionMode - None, Switch or Wireless.
         */
        int getConnectionMode();

        /**
         * @brief Gets the robot's current operating mode. 
         * @return A value of Manager::OperatingMode - Disabled, Autonomous, Driver or Free.
         */
        int getOperatingMode();

        /**
         * @brief Gets the color-coded string representation of the robot's current connection mode.
         */
        std::string getPrettyConnectionMode();

        /**
         * @brief Gets the color-coded string representation of the robot's current operating mode.
         */
        std::string getPrettyOperatingMode();

        /**
         * @brief Gets the color-coded string representation of the robot's current average motor temperatures.
         */
        std::string getPrettyTemperatures();

        /**
         * @brief Gets the color-coded string representation of the robot's current OkapiLib odometry state.
         */
        std::string getPrettyOdomState();

        /**
         * @brief Gets the string representation of the robot's current battery level and voltage.
         */
        std::string getPrettyBattery();

        /**
         * @brief Gets the string representation of the robot's current tracking encoder values.
         */
        std::string getPrettyEncoders();

        /**
         * @brief Sets the brake mode of the drive train.
         * @param brake_mode A pros::motor_brake_mode_e_t value.
         */
        void setDriveBrakeMode(pros::motor_brake_mode_e_t brake_mode);

        /**
         * @brief Sets the brake mode of the manipulators.
         * @param brake_mode A pros::motor_brake_mode_e_t value.
         */
        void setManipulatorBrakeMode(pros::motor_brake_mode_e_t brake_mode);
        
        /**
         * @brief Simulates a program restart; all sensors and odometry implementations are zeroed out and/or recalibrated.
         */
        void reinitialize();
};

/**
 * @brief Meant to be called as a Task. 
 * Suspends driver control, performs a ~3 second countdown (with controller vibration) then runs the autonomous function.
 * Resumes driver control once the autonomous function exits.
 */
void autonomousAsync();