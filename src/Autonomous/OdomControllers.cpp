#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/OdomControllers.hpp"

//TODO write PID loops using continuous odom feedback. 

//Need at minimum:
//Turn in place (relative and absolute)
//Turn to face point
//Drive to point (which subroutines turning to face a point)

//Would be cool-slash-nice to have:
//Drive to point in arc

QAngle constrainAngle(QAngle angle)
{
    printf("\nOriginal angle: ");
    printf(std::to_string(angle.convert(degree)).c_str());
    double x = angle.convert(degree);
    x = std::fmod(x, 360);
    if (x < 0) { x += 360; }
    printf("\nConstrained angle :");
    printf(std::to_string(x).c_str());
    return QAngle (x * degree);
}

QAngle getRobotHeading()
{
    return constrainAngle(drive_train->getState().theta);
}

void turnRelative(QAngle target_angle)
{
	double error = target_angle.convert(degree) - getRobotHeading().convert(degree);
    double threshold = 1.0;
	double integral;
	double derivative;
	double prevError;
	double kp = 0.01;
	double ki = 0;
	double kd = 0;

    PRINT("\n" + std::to_string(error));
    while(fabs(error) > threshold)
    {  
        PRINT(std::to_string(error));
        printf("\n");
        printf(std::to_string(error).c_str());
        error = target_angle.convert(degree) - getRobotHeading().convert(degree);
        integral = integral + error;

		if(error == 0 || fabs(error) >= target_angle.convert(degree))
        {
			integral = 0;
		}

		derivative = error - prevError;
		prevError = error;
		double p = error * kp;
		double i = integral * ki;
		double d = derivative * kd;

		double vel = p + i + d;

        if(error > 0)
        {
            drive_train->getModel()->left(vel);
            drive_train->getModel()->right(-vel);
        }
        else
        {
            drive_train->getModel()->left(-vel);
            drive_train->getModel()->right(vel);
        }

		pros::delay(15);
	}
    drive_train->getModel()->driveVector(0, 0);
}

void turnAbsolute(QAngle total_angle)
{
    QAngle absolute_angle = QAngle ((getRobotHeading().convert(degree) + total_angle.convert(degree)) * degree);
    absolute_angle = constrainAngle(absolute_angle);
    turnRelative(absolute_angle);
}

void turnToPoint(Point point)
{
    auto odom_state = drive_train->getState();
    double x_component = point.x.convert(meter) - odom_state.x.convert(meter);
    double y_component = point.y.convert(meter) - odom_state.y.convert(meter);
    double output = std::atan2(y_component, x_component) * 180 / PI;
    output = std::fmod((output + 360), 360);
    QAngle target_angle = QAngle(output * degree);
    target_angle = constrainAngle(target_angle);
    turnRelative(target_angle);
}

void driveToPoint(Point point)
{
    turnToPoint(point);
}
