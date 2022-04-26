#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

QAngle IMUOdometer::getRotation()
{
    return imu.get_rotation() * degree;
}

void IMUOdometer::updateState()
{
    previous_state = current_state;
    current_state = {getRotation(), encoder_left.get_value(), encoder_middle.get_value(), encoder_right.get_value()};
}

void IMUOdometer::updatePosition(Vector displacement_vector, QAngle heading)
{
    OdomState new_position;
    new_position.x = current_position.x + displacement_vector.x_component;
    new_position.y = current_position.y + displacement_vector.y_component;
    new_position.theta = heading;
    current_position = new_position;
}

void IMUOdometer::odometryLoop()
{
    std::uint32_t delay_timestamp = pros::millis();
    while(true)
    {
        if(getStatus() == Operational)
        {
            updateState();

            double parallel_encoder_change = ((current_state.left - previous_state.left) + (current_state.right - previous_state.right)) / 2.0;
            int parallel_direction = std::min(sgnum(parallel_encoder_change), 0); //0 indicates forwards, -1 indicates backwards
            QLength parallel_distance = ((parallel_encoder_change / 360.0) * wheel_circumfrence.convert(inch)) * inch;
            Vector parallel_displacement(parallel_distance, current_state.rotation + (180.0_deg * parallel_direction));

            int perpendicular_encoder_change = (current_state.middle - previous_state.middle);
            int perpendicular_direction = sgnum(perpendicular_encoder_change); //1 indicates right, -1 indicates left
            QLength perpendicular_distance = (perpendicular_encoder_change / 360.0) * wheel_circumfrence;
            Vector perpendicular_displacement(perpendicular_distance, current_state.rotation + (90.0_deg * perpendicular_direction));
            
            Vector total_displacement = parallel_displacement + perpendicular_displacement;

            updatePosition(total_displacement, current_state.rotation);
        }
        pros::Task::delay_until(&delay_timestamp, 10);
    }
}

IMUOdometer::IMUOdometer(pros::IMU& inertial, EncoderGroup& encoders, QLength tracking_wheel_diameter) : imu(inertial), encoder_left(encoders.left), encoder_middle(encoders.middle), encoder_right(encoders.right)
{
    wheel_circumfrence = (1_pi * tracking_wheel_diameter.convert(inch)) * inch;

    setPosition({0_ft, 0_ft, 0_deg});

    pros::Task odomLoop([this] { odometryLoop(); }, TASK_PRIORITY_DEFAULT + 2);
}

OdomState IMUOdometer::getPosition()
{
    return current_position;
}

void IMUOdometer::setPosition(OdomState position)
{
    current_position = position;
    imu.set_rotation(position.theta.convert(degree));
}

void IMUOdometer::reset()
{
    updateState();
    updateState();
    setPosition({0_ft, 0_ft, 0_deg});
}

int IMUOdometer::getStatus()
{
    if(imu.is_calibrating()) { return Calibrating; }
    else if(imu.get_rotation() == INFINITY) { return Error; }
    else { return Operational; }
}

std::string IMUOdometer::getPrettyPosition()
{
    if(getStatus() == Calibrating) { return "[#808080 CALIBRATING#]"; }
    else if(getStatus() == Error) { return "[#ff0000 ERROR#]"; }

    std::string x_coordinate = precise_string(current_position.x.convert(foot));
    std::string y_coordinate = precise_string(current_position.y.convert(foot));
    std::string rotation = precise_string(current_position.theta.convert(degree), 3);

    return "[" + x_coordinate + " | " + y_coordinate + " | " + rotation + "]";
}