#pragma once

class RobotManager
{
    private:

        std::vector<pros::Task> driverTasks;

        void autonomous_async()
        {
            suspendDriverControl();

            pros::delay(50);
            master.rumble("-");
            pros::delay(1000);
            master.rumble("-");
            pros::delay(1000);
            master.rumble("---");
            pros::delay(1000);

            autonomous();

            resumeDriverControl();
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

        void runAutonomousAsync()
        {
            pros::Task auton_async(autonomous_async);
        }

    };