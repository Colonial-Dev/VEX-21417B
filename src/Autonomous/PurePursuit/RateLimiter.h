class RateLimiter
{
    private:

        uint64_t last_called = pros::millis();
        QSpeed last_output;
    
    public:

        QSpeed getLimited(QSpeed input, QAcceleration limit)
        {
            double time_change = pros::millis() - last_called;
            double max_change = last_output.convert(mps) + time_change * limit.convert(mps2);
            last_called = pros::millis();
            double clamped = std::clamp(input.convert(mps), -max_change, max_change);
            QSpeed qspd = (clamped * mps);
            last_output = qspd;
            return qspd;
        }
};