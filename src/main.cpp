#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

#include "enums.h"
#include "ui_globals.h"
#include "auton_constants.h"
#include "menu.h"

using namespace std;
using namespace okapi;

void menuPrint(pros::Controller master)
{
    if(!pros::competition::is_connected()){pros::delay(50);}
    else{pros::delay(10);}
    master.set_text(0, 0, "> " + manager.GetCurrentItemName() + "                                        end");
}

//R1/L1 to page through options
//A to select
void advanced_auton_select(pros::Controller master)
{
  readyToLaunch = false;
  while(!readyToLaunch)
  {
    if(pros::competition::is_connected()) {return;}
    if(master.get_digital_new_press(DIGITAL_R1)) { manager.Page(true); }
    if(master.get_digital_new_press(DIGITAL_L1)) { manager.Page(false); }
    if(master.get_digital_new_press(DIGITAL_A)) { manager.DoOperation(); }
    if(master.get_digital_new_press(DIGITAL_B)) { manager.GotoLevel("Main"); }

    if(master.get_digital_new_press(DIGITAL_RIGHT)) { hotkeyMode = !hotkeyMode; }
    if(master.get_digital_new_press(DIGITAL_LEFT) && hotkeyMode) { liftAlignmentFlag = true; return; }
    if(master.get_digital_new_press(DIGITAL_UP) && hotkeyMode)
    {
      auton_test = true;
      pros::delay(50);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("---");
      pros::delay(1000);
      return;
    }

    if(displayUpdateFlag) { displayUpdateFlag = false; menuPrint(master); }
    pros::delay(2);
  }
  return;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize()
{
  manager.Update(levels);

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {8.7_ft, 0_ft, 0_deg}},
    "RushStraight"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {7_ft, 0_ft, 0_deg}},
    "PeekOut"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {6_ft, 0_ft, 0_deg}},
    "PeekOutShort"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {8.8_ft, 0_ft, 0_deg}},
    "RushMiddle"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {6.3_ft, 0_ft, 0_deg}},
    "RushMiddleShort"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {4.7_ft, 0_ft, 0_deg}},
    "BackGrab"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {10_ft, 0_ft, 0_deg}},
    "BackGrabLong"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {3_ft, 0_ft, 0_deg}},
    "BackGrabShort"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {1.5_ft, 0_ft, 0_deg}},
    "BackGrabTiny"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {4.9_ft, 0_ft, 0_deg}},
    "Park"
  );

  pros::Controller master(pros::E_CONTROLLER_MASTER);
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
  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == DirectRush)
    {
      driveTrain->turnAngle(40_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("RushStraight");
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(500);
      frontLiftLeft->setTarget(-500);
      frontLiftLeft->waitUntilSettled();
      pathFinder->setTarget("RushStraight", true);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(-200_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-700);
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      driveTrain->turnAngle(-200_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-700);
      rearLift->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny");
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(200_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("PeekOutShort");
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(130_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("RushMiddle");
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(700);
      frontLiftLeft->setTarget(-700);
      frontLiftLeft->waitUntilSettled();
      pathFinder->setTarget("RushMiddle", true);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(-130_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("Park", true);

    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == DirectRush)
    {
      pathFinder->setTarget("RushStraight");
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(500);
      frontLiftLeft->setTarget(-500);
      frontLiftLeft->waitUntilSettled();
      pathFinder->setTarget("PeekOutShort", true);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(-240_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabShort", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-700);
      rearLift->waitUntilSettled();
      pathFinder->setTarget("BackGrabShort");
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(280_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny", true);
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      pathFinder->setTarget("PeekOut");
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(-140_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabShort", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-800);
      rearLift->waitUntilSettled();
      driveTrain->turnAngle(-40_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("RushMiddle");
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(700);
      frontLiftLeft->setTarget(-700);
      frontLiftLeft->waitUntilSettled();
      pathFinder->setTarget("RushMiddle", true);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(150_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("Park", true);
      pathFinder->waitUntilSettled();
    }
  }
}

//OPCONTROL TASKS BELOW THIS POINT//

void infoPrint()
{
  pros::Controller master (CONTROLLER_MASTER);
  pros::Imu inertial(10);
  while(true)
  {
    if(displayUpdateFlag)
    {
      displayUpdateFlag = false;
      string throttle = std::to_string(throttleMultiplier * 100);
      throttle.resize(3);
      int trunc = (int)inertial.get_heading();
      string heading = std::to_string(trunc);
      master.set_text(0, 0, "THR " + throttle + "% / HDG " + heading);
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
