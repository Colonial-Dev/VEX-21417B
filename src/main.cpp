#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

//Global UI constants
//Not good design but what fucking ever
bool ready = false;
int auton_menustage = 0; //-1 indicates skip, 0 is side select, 1 is strat select, 2 is confirmation
int testauton = 0; //testing switch variable
int auton_side = 0; //0 indicates left, 1 indicates right
string auton_sidelabel = "Err";
int auton_variant = 0; //1-?, indicates what moves to make
string auton_varlabel = "Err";

//Global auton constants
std::shared_ptr<ChassisController> driveTrain =
  ChassisControllerBuilder()
    .withMotors({9, -19}, {-15, 13})
    .withGains
    (
      {0.001, 0, 0.0001}, // Distance controller gains
      {0.001, 0, 0.0001}, // Turn controller gains
      {0.001, 0, 0.0001}  // Angle controller gains
    )
    // Blue gearset, 4 in wheel diam, 9.5 in wheel track
    .withDimensions(AbstractMotor::gearset::blue, {{4_in, 9.5_in}, imev5BlueTPR})
    .build();

std::shared_ptr<AsyncMotionProfileController> pathFinder =
AsyncMotionProfileControllerBuilder()
  .withLimits({
    1.0, // Maximum linear velocity of the Chassis in m/s
    2.0, // Maximum linear acceleration of the Chassis in m/s/s
    10.0 // Maximum linear jerk of the Chassis in m/s/s/s
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

void live_status_screen()
{
  pros::lcd::set_text(0, "      21417A - Running");
  pros::lcd::set_text(1, "      21417A - Running");
  pros::lcd::set_text(2, "      21417A - Running");
  pros::lcd::set_text(3, "      21417A - Running");
  pros::lcd::set_text(4, "      21417A - Running");
  pros::lcd::set_text(5, "      21417A - Running");
  pros::lcd::set_text(6, "      21417A - Running");
  pros::lcd::set_text(7, "      21417A - Running");
}

void select_left()
{
  auton_side = 0;
  auton_sidelabel = "left";
  auton_menustage = 1;
}

void select_right()
{
  auton_side = 1;
  auton_sidelabel = "right";
  auton_menustage = 1;
}

void select_direct()
{
  auton_variant = 1;
  auton_varlabel = "Direct Rush";
  auton_menustage = 2;
}

void select_middle()
{
  auton_variant = 2;
  auton_varlabel = "Middle Rush";
  auton_menustage = 2;
}

void abort_auton()
{
  auton_menustage = 0;
}

void confirm_auton()
{
  ready = true;
}

void auton_select()
{
  pros::lcd::initialize();
  ready = false;
  while(!ready)
  {
    if(auton_menustage == 0)
    {
      pros::lcd::set_text(0, "      21417A - Auton Side?");
      pros::lcd::set_text(5, "");
      pros::lcd::set_text(6, "");
      pros::lcd::set_text(7, "<Left>                   <Right>");
      pros::lcd::register_btn0_cb(select_left);
      pros::lcd::register_btn2_cb(select_right);
    }
    else if(auton_menustage == 1)
    {
      pros::lcd::set_text(0, "      21417A - Auton Strategy?");
      pros::lcd::set_text(7, "<Direct Rush>      <Middle Rush>");
      pros::lcd::register_btn0_cb(select_direct);
      pros::lcd::register_btn2_cb(select_middle);
    }
    else if(auton_menustage == 2)
    {
      pros::lcd::set_text(0, "      21417A - Confirm Auton");
      pros::lcd::set_text(5, "Field side: " + auton_sidelabel);
      pros::lcd::set_text(6, "Strategy: " + auton_varlabel);
      pros::lcd::set_text(7, "<Abort>                <Confirm>");
      pros::lcd::register_btn0_cb(abort_auton);
      pros::lcd::register_btn2_cb(confirm_auton);
    }
    pros::delay(2);
  }
  pros::Task StatusDisp(live_status_screen);
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

 //LCD screen text box has a 32 char width and 7 lines. Font is monospaced.
void initialize()
{
  //auton_select();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
  auton_select();
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
  pathFinder->generatePath({
    {0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
    {6.8_ft, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
    "A" // Profile name
  );

  pathFinder->setTarget("A");
  pathFinder->waitUntilSettled();

  /*frontClamp->setTarget(360);
  rearLift->setTarget(1000);
  frontLiftLeft->setTarget(-2000);
  frontLiftRight->setTarget(2000);
  driveTrain->moveDistance(3_m);
  driveTrain->turnAngle(180_deg);
  driveTrain->moveDistance(3_m);*/
}

//OPCONTROL TASKS BELOW THIS POINT//

void refreshControllerScreen()
{
  double battLevel = pros::battery::get_capacity();

  string status = "";
  if(pros::competition::is_connected()){ status += "COMP // ";  }
  else{ status += "TEST // "; }
  if(pros::competition::is_autonomous() && !pros::competition::is_disabled()){ status += "AUTON"; }
  else if(pros::competition::is_disabled()){ status += "DSBLD"; }
  else{ status += "DRIVE"; }
  status += " // " + std::to_string(battLevel) + "%";

  pros::Controller master (CONTROLLER_MASTER);
  master.clear();
  master.print(1,18,status.c_str());
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
