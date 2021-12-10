#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

//Global UI constants
//Not good design but what fucking ever
bool ready = false;
string home_options[] = { "Run!", "Auton Select" , "Utilities", "Exit", "Left", "Right", "Direct Rush", "Middle Rush", "Confirm auton", "Abort auton", "Auto-alignment"};

int auton_menustage = 0;
int array_lowerbound = 0;
int array_upperbound = 3;
int array_index = 0;
bool updateneeded = true;

int auton_side = 2; //1 indicates left, 2 indicates right
string auton_sidelabel = "Err";
int auton_variant = 1; //1-?, indicates what moves to make
string auton_varlabel = "Err";

//Global auton constants
std::shared_ptr<ChassisController> driveTrain =
  ChassisControllerBuilder()
    .withMotors({9, -19}, {-15, 13})
    .withGains
    (
      {0.001, 0, 0.0001}, // Distance controller gains
      {0.001, 0, 0.0001}, // Turn controller gains
      {1, 0, 0.0001}  // Angle controller gains
    )
    // Blue gearset, 4 in wheel diam, 9.5 in wheel track
    .withDimensions(AbstractMotor::gearset::blue, {{4_in, 9.5_in}, imev5BlueTPR})
    .build();

std::shared_ptr<AsyncMotionProfileController> pathFinder =
AsyncMotionProfileControllerBuilder()
  .withLimits({
    50.0, // Maximum linear velocity of the Chassis in m/s
    5.0, // Maximum linear acceleration of the Chassis in m/s/s
    1000.0 // Maximum linear jerk of the Chassis in m/s/s/s
  })
  .withOutput(driveTrain)
  .buildMotionProfileController();

  //Initialize PID front lift controller
  std::shared_ptr<AsyncPositionController<double, double>> frontLiftRight =
  AsyncPosControllerBuilder()
    .withMotor(1)
    .build();

  //Initialize PID front lift controller
  std::shared_ptr<AsyncPositionController<double, double>> frontLiftLeft =
  AsyncPosControllerBuilder()
    .withMotor(5)
    .build();

  //Initialize clamp controller
  std::shared_ptr<AsyncPositionController<double, double>> frontClamp =
  AsyncPosControllerBuilder()
    .withMotor(8)
    .build();

  //Initialize PID rear lift controller
  std::shared_ptr<AsyncPositionController<double, double>> rearLift =
  AsyncPosControllerBuilder()
    .withMotor(6)
    .build();

void alignLifts()
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

  exit(0);
}

void page_options(int dir)
{
  if(dir == 0)
  {
    array_index++;
  }
  else
  {
    array_index--;
  }
  if(array_index > array_upperbound) { array_index = array_lowerbound; }
  if(array_index < array_lowerbound) { array_index = array_upperbound; }
  updateneeded = true;
}

void change_menustage(int stage)
{

  switch(stage)
  {
    case 1:
      array_lowerbound = 4;
      array_upperbound = 5;
      array_index = 4;
      break;

    case 2:
      array_lowerbound = 6;
      array_upperbound = 7;
      array_index = 6;
      break;

    case 3:
      array_lowerbound = 8;
      array_upperbound = 9;
      array_index = 8;
      break;

    case 4:
      array_lowerbound = 10;
      array_upperbound = 10;
      array_index = 10;
      break;

    default:
      array_lowerbound = 0;
      array_upperbound = 3;
      array_index = 0;
      break;
  }

  updateneeded = true;
  return;
}

void select_option() //this too
{
  switch(array_index)
  {
    case 0:
      ready = true;
      break;

    case 1:
      change_menustage(1);
      break;

    case 2:
      change_menustage(4);
      break;

    case 3:
      exit(0);
      break;

    case 4:
      auton_side = 1;
      change_menustage(2);
      break;

    case 5:
      auton_side = 2;
      change_menustage(2);
      break;

    case 6:
      auton_variant = 1;
      change_menustage(3);
      break;

    case 7:
      auton_variant = 2;
      change_menustage(3);
      break;

    case 8:
      ready = true;
      break;

    case 9:
      change_menustage(0);
      break;

    case 10:
      alignLifts();
      break;

    default:
      change_menustage(0);
      break;
  }

  updateneeded = true;
  return;
}

void menuPrint(pros::Controller master)
{
    if(!pros::competition::is_connected()){pros::delay(50);}
    else{pros::delay(10);}
    master.set_text(0, 0, "> " + home_options[array_index] + "                    ");
}

//R1/L1 to page through options
//A to select
void advanced_auton_select(pros::Controller master)
{
  ready = false;
  while(!ready)
  {
    if(pros::competition::is_connected()) {break;}


    if(master.get_digital_new_press(DIGITAL_R1))
    {
      page_options(0);
    }

    if(master.get_digital_new_press(DIGITAL_L1))
    {
      page_options(1);
    }

    if(master.get_digital_new_press(DIGITAL_A))
    {
      select_option();
    }

    if(master.get_digital_new_press(DIGITAL_B))
    {
      change_menustage(0);
    }

    if(updateneeded)
    {
      updateneeded = false;
      menuPrint(master);
    }

    pros::delay(2);
  }
  master.set_text(0, 0, "> Init complete...");
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
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  master.set_text(0, 0, "> Initializing...");
  advanced_auton_select(master);

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {8.5_ft, 0_ft, 0_deg}},
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
    {3.7_ft, 0_ft, 0_deg}},
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
    {1_ft, 0_ft, 0_deg}},
    "BackGrabTiny"
  );

  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},
    {4.5_ft, 0_ft, 0_deg}},
    "Park"
  );
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  //the below two lines should be uncommented for TESTING ONLY
  //auton_side = 1;
  //auton_variant = 2;
  if(auton_side == 1)
  {
    if(auton_variant == 1)
    {
      driveTrain->turnAngle(30_deg);
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
      driveTrain->turnAngle(-180_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-1000);
    }
    else if(auton_variant == 2)
    {
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      driveTrain->turnAngle(-180_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-800);
      rearLift->waitUntilSettled();
      pathFinder->setTarget("BackGrabTiny");
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(200_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("PeekOutShort");
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(130_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("RushMiddleShort");
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(700);
      frontLiftLeft->setTarget(-700);
      frontLiftLeft->waitUntilSettled();
      pathFinder->setTarget("RushMiddleShort", true);
      pathFinder->waitUntilSettled();
      driveTrain->turnAngle(-150_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("Park", true);
    }
  }

  else if(auton_side == 2)
  {
    if(auton_variant == 1)
    {
      pathFinder->setTarget("RushStraight");
      rearLift->setTarget(-5000);
      frontClamp->setTarget(-360);
      pathFinder->waitUntilSettled();
      frontClamp->setTarget(360);
      frontLiftRight->setTarget(500);
      frontLiftLeft->setTarget(-500);
      driveTrain->turnAngle(-110_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("BackGrab", true);
      pathFinder->waitUntilSettled();
      rearLift->setTarget(-1000);
      rearLift->waitUntilSettled();
      driveTrain->turnAngle(110_deg);
      driveTrain->waitUntilSettled();
      pathFinder->setTarget("Park", true);
    }
    else if(auton_variant == 2)
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
  float multiplier = 1.0f;
  pros::Controller master (CONTROLLER_MASTER);

  while (true) {
    if(master.get_digital_new_press(DIGITAL_A))
    {
      multiplier += 0.25f;
    }
    else if(master.get_digital_new_press(DIGITAL_Y))
    {
      multiplier -= 0.25f;
    }
    multiplier = std::clamp(multiplier, 0.0f, 1.0f);

    top_left.move(master.get_analog(ANALOG_LEFT_Y) * multiplier);
		btm_left.move(master.get_analog(ANALOG_LEFT_Y) * multiplier);
    top_right.move(master.get_analog(ANALOG_RIGHT_Y) * multiplier);
		btm_right.move(master.get_analog(ANALOG_RIGHT_Y) * multiplier);
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
	pros::Task Transmission(tankTransmission);
	pros::Task FrontLift(mainLiftControl);
	pros::Task Clamp(clampControl);
	pros::Task RearLift(rearLiftControl);
}
