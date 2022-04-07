#pragma once
#include "Autonomous/PurePursuit/RateLimiter.h"

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
pros::ADIDigitalOut back_piston ( {{13 , 'F'}});
pros::ADIDigitalOut clamp_piston ('G');
pros::ADIDigitalOut top_piston ( {{13 , 'E'}});

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
  .withSensors
  (
      ADIEncoder{'A', 'B', true}, //Left encoder
      ADIEncoder{'C', 'D'},  //Right encoder
      ADIEncoder{'E', 'F'}  //Middle encoder
  )
  .withDimensions({okapi::AbstractMotor::gearset::green, GEAR_RATIO}, {{4.125_in, 11.5_in}, okapi::imev5GreenTPR * GEAR_RATIO})
  //Specify odometry dimensions and encoder type
  .withOdometry({{2.875_in, 4.715_in, 3.5_in, 2.875_in}, quadEncoderTPR})
  .buildOdometry(); //Build an odometry-enabled chassis

EncoderGroup encoders = {left_encoder, middle_encoder, right_encoder};
InertialOdometer imu_odometer(inertial_sensor, encoders, 2.875_in);
  
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
