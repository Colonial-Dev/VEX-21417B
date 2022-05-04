#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Other/DriverControl.hpp"

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
		
		driver_control_gate.enterGate();
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
		driver_control_gate.enterGate();
	}
}

//B to close, Y to open
void frontClampControl()
{
  	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_B)) { clamp_piston.set_value(true); lock_piston.set_value(false); }
		else if(master.get_digital_new_press(DIGITAL_Y)) { clamp_piston.set_value(false); lock_piston.set_value(true); }
		driver_control_gate.enterGate();
  	}
}

//Down to close, right to open
void rearClampControl()
{
	while(true)
	{
		if(master.get_digital_new_press(DIGITAL_DOWN)) { back_piston.set_value(true); }
		else if(master.get_digital_new_press(DIGITAL_RIGHT)) { back_piston.set_value(false); }
		driver_control_gate.enterGate();
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
		else if(conveyorStatus == Forward) { conveyor_motor.move_velocity(-600); }
		else if(conveyorStatus == Reverse) { conveyor_motor.move_velocity(600); }
		
		driver_control_gate.enterGate();
	}
}


void initializeOpcontrol() 
{
	pros::Task transmission(splitTransmission);
	pros::Task frontLift(mainLiftControl);
	pros::Task frontClamp(frontClampControl);
	pros::Task rearClamp(rearClampControl);
	pros::Task conveyor(conveyorControl);
}
