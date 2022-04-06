#pragma once

class RobotManager
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

//Overwatch is an interface for managing the bot.
//You can use it to do things like test autons and reset the robot without shuttering the program first.
RobotManager overwatch;
RateLimiter limiter;

void autonomousAsync()
{
    pros::Controller master (CONTROLLER_MASTER);

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