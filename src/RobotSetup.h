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
int computedPaths = 0;

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

//280 RPM, 1.48 m/s
auto pathFinder = okapi::AsyncMotionProfileControllerBuilder()
  .withLimits({1.5, 5.5, 7.5}) //Max velocity, acceleration and jerk in m/s
  .withOutput(driveTrain->getModel(), {{4_in, 11.5_in}, okapi::imev5GreenTpr * GEAR_RATIO}, {okapi::AbstractMotor::gearset::green, GEAR_RATIO})
  .buildMotionProfileController();

//Initialize PID front lift controller
auto frontLift = okapi::AsyncPosControllerBuilder()
  .withMotor(8)
  .build();

//Initialize clamp controller
auto frontClamp = okapi::AsyncPosControllerBuilder()
  .withMotor(1)
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

//Precomputes all Okapi paths used in skills auton.
//Path name format: TYPE_ORIGIN_TARGET_SIDE, where:
//TYPE is the rough purpose of the path, such as rushing/back-grabbing a goal or field traversal,
//ORIGIN is the presumed starting point of the path,
//TARGET is the target of the path, such as a goal or general location on the field (e.g. the start position)
//SIDE is the field side the path is designed for. Not used in skills paths.
void initSkillsPaths()
{
  PATHGEN(0.75, 0, 0, "Jaunt_9")
  PATHGEN(1, 0, 0, "Jaunt_12");
  PATHGEN(1.25, 0, 0, "Jaunt_15")
  PATHGEN(1.5, 0, 0, "Jaunt_18");
  PATHGEN(2, 0, 0, "Jaunt_24")

  PATHGEN(3, 0, 0, "Rush_Start_AllianceGoal")
  PATHGEN(2, 0, 0, "Traverse_AllianceGoal_RamStart")
  PATHGEN(7.5, 0, 0, "Ram_SmallNeutral_EnemyZone")
  PATHGEN(7.5, 0, 0, "Ram_EnemyZone_LargeNeutral")
  PATHGEN(8, 0, 0, "Ram_FriendlyZone_SmallNeutral")
  PATHGEN(8.5, 0, 0, "Traverse_EnemyZone_FriendlyZone")
}

void initRightSimplePaths()
{
  PATHGEN(4, 0, 0, "Rush_Parking_SmallNeutral_Right")
}

void initLeftSimplePaths()
{
  PATHGEN(4, 1, 0, "Rush_Parking_SmallNeutral_Left")
}

void initRightDoublePaths()
{
  PATHGEN(4, 4, 45, "Rush_Parking_LargeNeutral_Right")
}

void initLeftDoublePaths()
{
  PATHGEN(4, 4, 45, "Rush_Parking_LargeNeutral_Left")
}

void initRightStackPaths()
{
  PATHGEN(1, 0, 0, "Peek_Out")
  PATHGEN(2, 0, 0, "Traverse_Parking_Balance_Right")
}

void initLeftStackPaths()
{
  PATHGEN(1, 0, 0, "Peek_Out")
  PATHGEN(4, 0, 0, "Traverse_Parking_Balance_Left")
}

//Precomputes Okapi paths used in standard auton.
void initPaths(int side, int strategy)
{
    PATHGEN(3, 0, 0, "TestPath")
    PATHGEN(4, 3, 30, "TestPath_MG")
    PATHGEN(4, 8, 0, "TestPath_MGB")

    if(side == Skills) { initSkillsPaths(); return; }
    
    if(side == Right)
    {
      switch (strategy)
      {
        case SimpleRush:
        {
          initRightSimplePaths();
        }

        case DoubleRush:
        {
          initRightSimplePaths();
          initRightDoublePaths();
        }

        case StackRush:
        {
          initRightSimplePaths();
          initRightStackPaths();
        }

        default:
        {
          return;
        }
      }
      return;
    }

          
    if(side == Left)
    {
      switch (strategy)
      {
        case SimpleRush:
        {
          initLeftSimplePaths();
        }

        case DoubleRush:
        {
          initLeftSimplePaths();
          initLeftDoublePaths();
        }

        case StackRush:
        {
          initLeftSimplePaths();
          initLeftStackPaths(); 
        }

        default:
        {
          return;
        }
      }
      return;
    }
}
