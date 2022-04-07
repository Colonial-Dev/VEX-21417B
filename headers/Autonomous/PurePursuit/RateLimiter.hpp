#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"

class RateLimiter
{
    private:

        uint64_t last_called = pros::millis();
        QSpeed last_output;
    
    public:

        QSpeed getLimited(QSpeed input, QAcceleration limit);

};