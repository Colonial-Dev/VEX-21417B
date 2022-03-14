#pragma once
#include "Enums.h"
#include "Macros.h"

using namespace okapi::literals;

pros::Controller master (CONTROLLER_MASTER);

pros::Motor right_back (12);
pros::Motor right_middle (15, true);
pros::Motor right_front (16);
pros::Motor left_back (19, true);
pros::Motor left_middle (6);
pros::Motor left_front (9, true);

pros::Motor arm_motor (8);
pros::Motor claw_motor (1);

pros::IMU inertial_sensor (17);

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
  right_back.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_middle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_front.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  left_back.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  left_middle.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  left_front.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

  arm_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  claw_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}
