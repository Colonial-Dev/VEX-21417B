class RateLimiter
{
    private:

        uint64_t last_called = pros::millis();
        double last_output;
    
    public:

        double getLimited(double input, double limit)
        {
            double time_change = pros::millis() - last_called;
            double max_change = last_output + time_change * limit;
            last_called = pros::millis();
            return std::clamp(input - last_output, -max_change, max_change);
        }
};