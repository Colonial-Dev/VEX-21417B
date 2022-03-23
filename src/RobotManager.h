class RobotManager
{
    private:

        std::vector<pros::Task> driverTasks;

    public:

        void registerDriverTask(pros::Task newTask)
        {
            newTask.notify();
            driverTasks.push_back(newTask);
        }

        void suspendDriverControl()
        {
            for(int i = 0; i < driverTasks.size(); i++)
            {
                driverTasks.at(i).notify_clear();
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