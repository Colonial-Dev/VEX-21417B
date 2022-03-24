#pragma once

struct OdometryState
{
    double x_pos;
    double y_pos;
    double heading;
};

class InertialOdometry
{
    private:

        double x_pos;
        double y_pos;
        double heading;

        double l_enc_prev = 0;
        double r_enc_prev = 0;

        void updatePosition()
        {
            uint32_t now = pros::millis();
            while(true)
            {
                heading = inertial_sensor.get_heading();
                int left_enc_current = left_encoder.get_value();
                int right_enc_current = right_encoder.get_value();

                double distance_travelled = ((left_enc_current - l_enc_prev) * (right_enc_current - r_enc_prev)) / 2;
                x_pos += distance * std::cos(heading);
                y_pos += distance * std::sin(heading);
                pros::delay_until(&now, 10);
            }
        }
    
    public:
        
        InertialOdometry(int origin_offset_x = 0, int origin_offset_y = 0)
        {
            x_pos += origin_offset_x;
            y_pos += origin_offset_y;
            pros::Task updateTask (updatePosition, NULL, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "InertialOdomUpdater");
        }

        OdometryState getState()
        {
            return OdometryState state = {x_pos, y_pos, heading};
        }

};