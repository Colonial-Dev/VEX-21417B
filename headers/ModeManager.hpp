#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"

class ModeManager
{
    private:

        std::vector<pros::Task> driverTasks;

    public:

        void setBrakeMode(pros::motor_brake_mode_e_t brake_mode);

        void registerDriverTask(pros::task_t task);

        void suspendDriverControl();

        void resumeDriverControl();

        //Resets all relative sensor values to default, simulating a restart of the program.
        void reinitialize();
};

void autonomousAsync();