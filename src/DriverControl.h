#pragma once

//Axis 3 is translation, axis 1 is rotation
void splitTransmission()
{
	while (true) 
	{	
		right_front.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X))	;
		right_middle.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X));
    	right_back.move(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_RIGHT_X));

    	left_front.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X));
		left_middle.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X));
		left_back.move(master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_RIGHT_X));
		
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
	}
}

//L1 up, L2 down
void mainLiftControl()
{
  	while(true)
	{
		if(master.get_digital(DIGITAL_L1)) { arm_motor.move_velocity(200); }
		else if(master.get_digital(DIGITAL_L2)) { arm_motor.move_velocity(-200); }
		else { arm_motor.move_velocity(0); }
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
	}
}

//B to close, Y to open
void frontClampControl()
{
  	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_B)) { clamp_piston.set_value(true); }
		else if(master.get_digital_new_press(DIGITAL_Y)) { clamp_piston.set_value(false); }
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
  	}
}

//Down to close, right to open
void rearClampControl()
{
	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_DOWN)) { back_piston.set_value(true); }
		else if(master.get_digital_new_press(DIGITAL_RIGHT)) { back_piston.set_value(false); }
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
	}
}

//X to close, A to open
void topClampControl()
{
	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_X)) { top_piston.set_value(true); }
		else if(master.get_digital_new_press(DIGITAL_A)) { top_piston.set_value(false); }
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
	}
}

//R1 forward, R2 reverse
void conveyorControl()
{
	int conveyorStatus = Idle;

	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_R1))
		{
			if(conveyorStatus == Idle || conveyorStatus == Reverse) { conveyorStatus = Forward; }
			else { conveyorStatus = Idle; }
		}
		else if(master.get_digital_new_press(DIGITAL_R2))
		{
			if(conveyorStatus == Idle || conveyorStatus == Forward) { conveyorStatus = Reverse; }
			else { conveyorStatus = Idle; }
		}

		if(conveyorStatus == Idle) { conveyor_motor.move_velocity(0); }
		else if(conveyorStatus == Forward) { conveyor_motor.move_velocity(600); }
		else if(conveyorStatus == Reverse) { conveyor_motor.move_velocity(-600); }
		
		pros::Task::notify_take(true, TIMEOUT_MAX);
		pros::delay(2);
	}
}


void opcontrol() 
{
	pros::Task transmission(splitTransmission);
	pros::Task frontLift(mainLiftControl);
	pros::Task frontClamp(frontClampControl);
	pros::Task rearClamp(rearClampControl);
	pros::Task topClamp(topClampControl);
	pros::Task conveyor(conveyorControl);

	overwatch.registerDriverTask(transmission);
	overwatch.registerDriverTask(frontLift);
	overwatch.registerDriverTask(frontClamp);
	overwatch.registerDriverTask(rearClamp);
	overwatch.registerDriverTask(topClamp);
	overwatch.registerDriverTask(conveyor);
}
