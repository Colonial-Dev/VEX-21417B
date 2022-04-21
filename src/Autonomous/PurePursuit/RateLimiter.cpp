#include "robokauz/PROS.hpp"
#include "robokauz/Autonomous/PurePursuit/RateLimiter.hpp"

QSpeed RateLimiter::getLimited(QSpeed input, QAcceleration limit)
{
    double time_change = pros::millis() - last_called;
    last_called = pros::millis();
    QSpeed max_velocity  = (last_output.convert(mps) + (time_change / 1000) * limit.convert(mps2)) * mps;

    QSpeed output;
    if(input > max_velocity) { output = max_velocity; }
    else{ output = input; }

    last_output = output;
    return output;
}