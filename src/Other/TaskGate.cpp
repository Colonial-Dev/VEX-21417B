#include "robokauz/PRELUDE.hpp"
#include "robokauz/Other/TaskGate.hpp"

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