#pragma once

class RobotManager
{
    private:

        std::vector<pros::Task> driverTasks;

        void runAuton()
        {
            pros::Controller master (CONTROLLER_MASTER);

            pros::delay(50);
            master.rumble("-");
            pros::delay(1000);
            master.rumble("-");
            pros::delay(1000);
            master.rumble("---");
            pros::delay(1000);

            autonomous();
        }

    public:

        void registerDriverTask(pros::task_t task)
        {
            pros::Task newTask(task);
            newTask.notify();
            driverTasks.push_back(newTask);
        }

        void suspendDriverControl()
        {
            for(int i = 0; i < driverTasks.size(); i++)
            {
                driverTasks.at(i).notify_ext(0, NOTIFY_ACTION_OWRITE, NULL);
            }
        }

        void resumeDriverControl()
        {
            for(int i = 0; i < driverTasks.size(); i++)
            {
                driverTasks.at(i).notify();
            }
        }

        void testAutonomous()
        {
            suspendDriverControl();
            runAuton();
            resumeDriverControl();
        }

        //Resets all relative sensor values to default, simulating a restart of the program.
        void reinitialize()
        {
            inertial_sensor.reset();
            left_encoder.reset();
            middle_encoder.reset();
            right_encoder.reset();
            drive_train.reset();
        }

};