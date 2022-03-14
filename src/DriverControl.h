#pragma once

void tankTransmission()
{
	while (true) 
	{	
    	right_back.move(master.get_analog(ANALOG_RIGHT_Y) * throttleMultiplier);
		right_middle.move(master.get_analog(ANALOG_RIGHT_Y) * throttleMultiplier);
		right_front.move(master.get_analog(ANALOG_RIGHT_Y) * throttleMultiplier);
    	
		left_back.move(master.get_analog(ANALOG_LEFT_Y) * throttleMultiplier);
		left_middle.move(master.get_analog(ANALOG_LEFT_Y) * throttleMultiplier);
		left_front.move(master.get_analog(ANALOG_LEFT_Y) * throttleMultiplier);

		pros::delay(2);
	}
}

//port 6 - back lift
//port 1 - front lift right
//port 5 - front lift left
//port 8 - claw motor
void mainLiftControl()
{
  	//R1 up, R2 down
  	while(true)
	{
		if(master.get_digital(DIGITAL_R1))
		{
			arm_motor.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_R2))
		{
			arm_motor.move_velocity(-200);
		}
		else
		{
			arm_motor.move_velocity(0);
		}
		pros::delay(2);
	}
}

void clampControl()
{
  	//R1 open , R2 close
  	while(true)
	{
		if(master.get_digital(DIGITAL_L2))
		{
			claw_motor.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_L1))
		{
			claw_motor.move_velocity(-200);
		}
		else{
			claw_motor.move_velocity(0);
		}
		pros::delay(2);
  	}
}

void opcontrol() 
{
	brainPrint("#0000ff [INFO]# Initializing operator control...");
	pros::Task Transmission(tankTransmission);
	pros::Task FrontLift(mainLiftControl);
	pros::Task Clamp(clampControl);
}
