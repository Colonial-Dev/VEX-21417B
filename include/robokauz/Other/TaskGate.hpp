#pragma once
#include "robokauz/PRELUDE.hpp"

//Simple synchronization primitive. Can either be open or closed.
//Tasks call enterGate each time they loop:
//If the gate is closed, the call blocks them until it is open.
//If it is open, they are blocked once (10ms) and allowed to continue.
class TaskGate
{
    private:

        bool is_open = true;

    public:

        void openGate();

        void closeGate();
        
        void toggleGate();

        void enterGate();
};