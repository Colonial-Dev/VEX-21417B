#pragma once
#include "Enums.h"
#include "Macros.h"

using namespace okapi::literals;

pros::Controller master (CONTROLLER_MASTER); //Acquire the controller for global use

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
pros::ADIDigitalOut back_piston ('E');
pros::ADIDigitalOut clamp_piston ('G');
pros::ADIDigitalOut top_piston ('F');

//Initialize sensors
pros::IMU inertial_sensor (14);
pros::ADIEncoder left_encoder ('A', 'B');
pros::ADIEncoder right_encoder ('C', 'D');
pros::ADIAnalogIn potentiometer ('H'); 



double GEAR_RATIO = 60.0/84.0;

auto driveTrain = okapi::ChassisControllerBuilder()
  .withMotors({19, 6, 9}, {12, 15, 16})
  .withGains
  (
    {0.001, 0, 0.0001}, // Distance controller gains
    {0.0032, 0.0003, 0.0001}, // Turn controller gains
    {0.7, 0.001, 0.0001}  // Angle controller gains
  )
  //Green gearset, 4 in wheel diam, 11.5 in wheel track
  .withDimensions({okapi::AbstractMotor::gearset::green, GEAR_RATIO}, {{4_in, 11.5_in}, okapi::imev5GreenTpr * GEAR_RATIO})
  .build();
  
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
