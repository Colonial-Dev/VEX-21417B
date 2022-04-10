#pragma once
#include "robokauz/PROS.hpp"
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

        std::vector<pros::Task> driverTasks;

        std::string getTemperatureColored(double temp);

    public:

        int getConnectionMode();

        int getOperatingMode();

        std::string getPrettyConnectionMode();

        std::string getPrettyOperatingMode();

        std::string getPrettyTemperatures();

        std::string getPrettyOdomState();

        std::string getPrettyBattery();

        std::string getPrettyEncoders();

        void setBrakeMode(pros::motor_brake_mode_e_t brake_mode);

        void registerDriverTask(pros::task_t task);

        void suspendDriverControl();

        void resumeDriverControl();

        //Resets all relative sensor values to default, simulating a restart of the program.
        void reinitialize();
};

void autonomousAsync();