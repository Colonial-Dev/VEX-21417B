#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

#include "Enums.h"
#include "InterfaceGlobals.h"
#include "AutonomousConstants.h"
#include "Macros.h"
#include "BrainDisplay.h"

using namespace std;
using namespace okapi;

void initialize() 
{
  splashDisplay();
  manager.Update(levels);
  brainPrint("Menu system backend #00ff00 [OK]#");
  brainPrint("#ffff00 [WARN]# Starting auton path precomputation!");
  initPaths();
  brainPrint("Auton paths #00ff00 [OK]#");

  pros::Controller master(pros::E_CONTROLLER_MASTER);
  brainPrint("Broadcasting menu system interface...");
  advanced_auton_select(master);

  if(autonTestFlag) { autonomous(); exit(0); }

  if(liftAlignmentFlag)
  {
    rearLift->setTarget(5000);
    frontClamp->setTarget(-360);
    frontLiftLeft->setTarget(-4000);
    frontLiftRight->setTarget(4000);

    pros::delay(2000);

    frontLiftLeft->reset();
    frontLiftRight->reset();

    frontLiftLeft->tarePosition();
    frontLiftRight->tarePosition();

    frontLiftLeft->setTarget(2750);
    frontLiftRight->setTarget(-2750);

    pros::delay(3000);
    abort(); //fucking STOP
  }

  while(true)
  {
      if(master.get_digital_new_press(DIGITAL_DOWN)){return;}
      if(pros::competition::is_connected()){return;}
      pros::delay(2);
  }
}

void autonomous()
{
  PATH("Test_None_Both")
  WAIT
  TURN(90)
  PATH("Test_None_Both")
  WAIT
  TURN(90)
  PATH("Test_None_Both")
  WAIT
  TURN(90)
  PATH("Test_None_Both")
  WAIT
  TURN(90)
  return;

  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == DirectRush)
    {
      
    }
    else if(targetAutonStrategy == MiddleRush)
    {

    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == DirectRush)
    {

    }
    else if(targetAutonStrategy == MiddleRush)
    {

    }
  }

}

//OPCONTROL TASKS BELOW THIS POINT//

void infoPrint()
{
  pros::Controller master (CONTROLLER_MASTER);
  while(true)
  {
    if(displayUpdateFlag)
    {
      displayUpdateFlag = false;
      string throttle = std::to_string(throttleMultiplier * 100);
      throttle.resize(3);
      master.set_text(0, 0, "THR " + throttle);
    }
    pros::delay(50);
  }
  return;
}

//port 9 is top left
//port 15 is top right
//port 19 is bottom left
//port 13 is bottom right
void tankTransmission(){
	pros::Motor top_left (9);
	pros::Motor top_right (15, true);
	pros::Motor btm_left (19, true);
	pros::Motor btm_right (13);
  top_left.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  top_right.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  btm_left.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  btm_right.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  pros::Controller master (CONTROLLER_MASTER);

  while (true) {
    if(master.get_digital_new_press(DIGITAL_A))
    {
      throttleMultiplier += 0.2f;
      displayUpdateFlag = true;
    }
    else if(master.get_digital_new_press(DIGITAL_Y))
    {
      throttleMultiplier -= 0.2f;
      displayUpdateFlag = true;
    }
    throttleMultiplier = std::clamp(throttleMultiplier, 0.2f, 1.0f);

    top_left.move(master.get_analog(ANALOG_LEFT_Y) * throttleMultiplier);
		btm_left.move(master.get_analog(ANALOG_LEFT_Y) * throttleMultiplier);
    top_right.move(master.get_analog(ANALOG_RIGHT_Y) * throttleMultiplier);
		btm_right.move(master.get_analog(ANALOG_RIGHT_Y) * throttleMultiplier);
    pros::delay(2);
  }
}

//port 6 - back lift
//port 1 - front lift right
//port 5 - front lift left
//port 8 - claw motor
void mainLiftControl(){
  //R1 up, R2 down
	pros::Motor right_motor (1);
	pros::Motor left_motor (5);
	right_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	left_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Controller master (CONTROLLER_MASTER);
  while(true)
	{
		if(master.get_digital(DIGITAL_R1))
		{
			right_motor.move_velocity(200);
			left_motor.move_velocity(-200);
		}
		else if(master.get_digital(DIGITAL_R2))
		{
			right_motor.move_velocity(-200);
			left_motor.move_velocity(200);
		}
		else{
			right_motor.move_velocity(0);
			left_motor.move_velocity(0);
		}
		pros::delay(2);
  }
}

void clampControl(){
  //R1 open , R2 close
	pros::Motor motor (8);
	motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Controller master (CONTROLLER_MASTER);
  while(true)
	{
		if(master.get_digital(DIGITAL_L2))
		{
			motor.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_L1))
		{
			motor.move_velocity(-200);
		}
		else{
			motor.move_velocity(0);
		}
		pros::delay(2);
  }
}

void rearLiftControl(){
  //X up, B down
	pros::Motor motor (6);
	motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Controller master (CONTROLLER_MASTER);
  while(true)
	{
		if(master.get_digital(DIGITAL_X))
		{
			motor.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_B))
		{
			motor.move_velocity(-200);
		}
		else{
			motor.move_velocity(0);
		}
		pros::delay(2);
  }
}

void opcontrol() {
  pros::Controller master (CONTROLLER_MASTER);
  displayUpdateFlag = true;
	pros::Task Transmission(tankTransmission);
	pros::Task FrontLift(mainLiftControl);
	pros::Task Clamp(clampControl);
	pros::Task RearLift(rearLiftControl);
  pros::Task Disp(infoPrint);
}
