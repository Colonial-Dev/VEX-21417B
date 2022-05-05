#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/Autonomous/Vector2D.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

QAngle getRobotHeading()
{
    return (inertial_sensor.get_rotation()) * degree;
}

void turnRelative(QAngle target_angle, bool goal)
{
	double error = target_angle.convert(degree) - getRobotHeading().convert(degree);
    double threshold_count = 0;
	double integral;
	double derivative;
	double prevError;
	double kp = goal ? 1.5 : 1.7;
	double ki = goal ? 0.000045 : 0.0000045;
	double kd = goal ? 16 : 5;

    while(true)
    {  
        error = target_angle.convert(degree) - getRobotHeading().convert(degree);
        integral = (std::fabs(error) <= 15.0) ? (integral + error) : integral;
        integral = (error == 0 || sgnum(error) != sgnum(prevError)) ? 0 : integral;
		derivative = error - prevError;
		prevError = error;

        if(std::fabs(error) <= 2.0 && std::fabs(derivative) <= 4.0) { threshold_count++; }
        else { threshold_count = 0; }

        if(threshold_count >= 10) { break; }

		double p = error * kp;
		double i = integral * ki;
		double d = derivative * kd;

		double vel = p + i + d;

		right_back.move_velocity(-vel);
        right_middle.move_velocity(-vel);
        right_front.move_velocity(-vel);

        left_back.move_velocity(vel);
        left_middle.move_velocity(vel);
        left_front.move_velocity(vel);

		pros::delay(10);
	}
    drive_controller->tank(0, 0);
}