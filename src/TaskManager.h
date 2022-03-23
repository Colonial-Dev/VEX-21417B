#pragma once

class TaskManager
{
    private:

        std::vector<pros::Task> driverTasks;

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

    };