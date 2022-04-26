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

//More complex derivative of a basic gate. Can be either closed or in a certain "access state."
//The access state is an enum value (simple integer, not class), known as a "group ID."
//When tasks call enterGate, they provide their group ID (ideally) from the same enum.
//If their group ID matches the current access state, they are blocked once (10ms) and allowed to continue.
//If their group ID does NOT match the current access state, they are blocked until it does.
//Hence the name mutex/"mutually exclusive" - only one task group is allowed through at any given time.
//NB enum values < 0 will shut the gate to ALL group IDs, even if they are also < 0.
class MutexTaskGate
{
    private:

        int is_open_for = -1;

    public:

        void openGateFor(int group_id);
        
        void closeGate();

        void enterGate(int group_id);
};