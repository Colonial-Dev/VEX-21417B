#pragma once

using namespace okapi::literals;

//Robot state enums
enum ConveyorStatus
{
  Idle,
  Forward,
  Reverse
};

enum AutonSide
{
  Left,
  Right,
  Skills,
  None
};

enum AutonStrat
{
  SimpleRush, //Go straight forwards, grab the mobile goal, and return.
  DoubleRush, //Go straight forwards, grab the mobile goal, turn and grab the colored goal, and finally return.
  StackRush, //Grab the colored goal as a counterweight, then pick up the middle mobile goal and return.
};

//Create overwatch insttance
TaskManager overwatch;

//Acquire the controller for global use
pros::Controller master (CONTROLLER_MASTER);

//Initialize drivetrain motors
pros::Motor right_back (12, true);
pros::Motor right_middle (15);
pros::Motor right_front (16, true);
pros::Motor left_back (19);
pros::Motor left_middle (6, true);
pros::Motor left_front (9);

//Initialize manipulator motors/pneumatics
pros::Motor arm_motor (5);
pros::Motor conveyor_motor (8);
pros::ADIDigitalOut back_piston ( {{7 , 'F'}});
pros::ADIDigitalOut clamp_piston ('G');
pros::ADIDigitalOut top_piston ( {{7 , 'E'}});

//Initialize sensors
pros::IMU inertial_sensor (14);
pros::ADIEncoder left_encoder ('A', 'B');
pros::ADIEncoder right_encoder ('C', 'D');
pros::ADIEncoder middle_encoder ('E', 'F');
pros::ADIAnalogIn potentiometer ('H'); 

//Drivetrain gear ratio constant
double GEAR_RATIO = 60.0/84.0;

auto drive_train = okapi::ChassisControllerBuilder()
  .withMotors({19, 6, 9}, {12, 15, 16})
  .withGains
  (
      {0.001, 0, 0.0001}, //Distance gains
      {0.0032, 0.0003, 0.0001}, //Turn gains
      {0.7, 0.001, 0.0001}  //Angle gains
  )
  .withSensors
  (
      ADIEncoder{'A', 'B', true}, //Left encoder (inverted)
      ADIEncoder{'C', 'D'},  //Right encoder
      ADIEncoder{'E', 'F', true}  //Middle encoder (inverted)
  )
  .withDimensions({okapi::AbstractMotor::gearset::green, GEAR_RATIO}, {{4_in, 11_in}, okapi::imev5GreenTPR * GEAR_RATIO})
  //Specify odometry dimensions and encoder type
  .withOdometry({{3.819_in, 5.16495_in, 3.5_in, 2.75_in}, quadEncoderTPR})
  .buildOdometry(); //Build an odometry-enabled chassis
  
void setupBrakeModes()
{
  right_back.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_middle.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_front.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  left_back.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  left_middle.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  left_front.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  conveyor_motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}
