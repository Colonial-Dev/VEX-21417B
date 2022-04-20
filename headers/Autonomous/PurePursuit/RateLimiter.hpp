#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"

/**
 * @brief Simple object used to rate-limit target velocities.
 */
class RateLimiter
{
    private:

        /**
         * @brief The timestamp at which the rate limiter was last called, in milliseconds.
         */
        uint64_t last_called = pros::millis();

        /**
         * @brief The last velocity value output by the rate limiter.
         */
        QSpeed last_output;
    
    public:

        /**
         * @brief Gets a limited target velocity, given an unlimited target velocity and max acceleration.
         * @param input The unlimited target velocity.
         * @param limit The max allowable acceleration.
         * @return The limited target velocity.
         */
        QSpeed getLimited(QSpeed input, QAcceleration limit);

};