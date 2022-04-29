#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

//TODO write PID loops using continuous odom feedback. 

//Need at minimum:
//Turn in place (relative and absolute)
//Turn to face point
//Drive to point (which subroutines turning to face a point)

//Would be cool-slash-nice to have:
//Drive to point in arc

QAngle getRobotHeading()
{
    return (inertial_sensor.get_rotation()) * degree;
}

void turnRelative(QAngle target_angle)
{
	double error = target_angle.convert(degree) - getRobotHeading().convert(degree);
    double threshold_count = 0;
	double integral;
	double derivative;
	double prevError;
	double kp = 1.3;
	double ki = 0.0000045;
	double kd = 8;

    while(true)
    {  
        error = target_angle.convert(degree) - getRobotHeading().convert(degree);
        integral = (std::fabs(error) <= 15.0) ? (integral + error) : integral;
        integral = (error == 0 || sgnum(error) != sgnum(prevError)) ? 0 : integral;
		derivative = error - prevError;
		prevError = error;

        if(std::fabs(error) < 4.0 && std::fabs(derivative) < 4.0) { threshold_count++; }
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
    drive_train->getModel()->driveVector(0, 0);
}

void turnAbsolute(QAngle total_angle)
{
    QAngle absolute_angle = getRobotHeading() + total_angle;
    turnRelative(absolute_angle);
}

void turnToPoint(Point point)
{
    auto odom_state = drive_train->getState();
    double x_component = point.x.convert(meter) - odom_state.x.convert(meter);
    double y_component = point.y.convert(meter) - odom_state.y.convert(meter);
    double output = std::atan2(y_component, x_component) * 180 / pi;
    output = std::fmod((output + 360), 360);
    QAngle target_angle = QAngle(output * degree);
    target_angle = constrainAngle360(target_angle);
    turnRelative(target_angle);
}
