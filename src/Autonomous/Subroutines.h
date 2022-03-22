#pragma once

void autonomous_async()
{
  driverControlLocked = true;
  
  pros::delay(50);
  master.rumble("-");
  pros::delay(1000);
  master.rumble("-");
  pros::delay(1000);
  master.rumble("---");
  pros::delay(1000);

  autonomous();

  driverControlLocked = false;
}

void inertialTurn(double angle)
{
	double error = angle - inertial_sensor.get_rotation();
    double threshold = 0.93 + (0.0025 * fabs(error)) + fabs(error * 0.001);
	double integral;
	double derivative;
	double prevError;
	double kp = 1.2;
	double ki = 0.0025;
	double kd = 7.5;

    while(fabs(error) > threshold)
    {  
        error = angle - inertial_sensor.get_rotation();
        integral  = integral + error;

		if(error == 0 || fabs(error) >= angle)
        {
			integral = 0;
		}

		derivative = error - prevError;
		prevError = error;
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

		pros::delay(15);
	}
    right_back.move_velocity(0);
    right_middle.move_velocity(0);
    right_front.move_velocity(0);

    left_back.move_velocity(0);
    left_middle.move_velocity(0);
    left_front.move_velocity(0);
}