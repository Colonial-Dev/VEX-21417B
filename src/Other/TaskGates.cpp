#include "robokauz/PRELUDE.hpp"
#include "robokauz/Other/TaskGates.hpp"

void TaskGate::openGate()
{
    is_open = true;
}

void TaskGate::closeGate()
{
    is_open = false;
}

void TaskGate::toggleGate()
{
    is_open = !is_open;
}

void TaskGate::enterGate()
{
    do
    {
        pros::delay(10);
    }
    while(!is_open);
}

void MutexTaskGate::openGateFor(int group_id)
{
    is_open_for = group_id;
}

void MutexTaskGate::closeGate()
{
    is_open_for = -1;
}

void MutexTaskGate::enterGate(int group_id)
{
    do
    {
        pros::delay(10);
    }
    while(group_id != is_open_for || is_open_for < 0);
}