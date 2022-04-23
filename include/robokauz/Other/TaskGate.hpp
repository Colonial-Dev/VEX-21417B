#pragma once
#include "robokauz/PRELUDE.hpp"

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