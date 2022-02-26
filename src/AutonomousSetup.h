#pragma once
#include "Enums.h"
#include "Macros.h"

using namespace okapi::literals;

pros::Imu inertialSensor (21);
double GEAR_RATIO = 18.0/24.0;
int computedPaths = 0;

auto driveTrain = okapi::ChassisControllerBuilder()
  .withMotors({9, 19}, {-15, -13})
  .withGains
  (
    {0.001, 0, 0.0001}, // Distance controller gains
    {0.003, 0.0004, 0.0001}, // Turn controller gains
    {0.7, 0.001, 0.0001}  // Angle controller gains
  )
  // Green gearset, 4 in wheel diam, 11 in wheel track
  .withDimensions({okapi::AbstractMotor::gearset::green, GEAR_RATIO}, {{4_in, 11_in}, okapi::imev5GreenTpr * GEAR_RATIO})
  .build();

//Bot weighs ~6.577089kg
auto pathFinder = okapi::AsyncMotionProfileControllerBuilder()
  .withLimits({4, 5.8, 7.5})
  .withOutput(driveTrain->getModel(), {{4_in, 11_in}, okapi::imev5GreenTpr * GEAR_RATIO}, {okapi::AbstractMotor::gearset::green, GEAR_RATIO})
  .buildMotionProfileController();

//Initialize PID front lift controller
auto frontLiftRight = okapi::AsyncPosControllerBuilder()
  .withMotor(1)
  .build();

  //Initialize PID front lift controller
auto frontLiftLeft = okapi::AsyncPosControllerBuilder()
    .withMotor(5)
    .build();

  //Initialize clamp controller
  auto frontClamp = okapi::AsyncPosControllerBuilder()
    .withMotor(8)
    .build();

  //Initialize PID rear lift controller
  auto rearLift = okapi::AsyncPosControllerBuilder()
    .withMotor(6)
    .build();
  
  //Precomputes all Okapi paths used in skills auton.
  //Path name format: TYPE_ORIGIN_TARGET_SIDE, where:
  //TYPE is the rough purpose of the path, such as rushing/back-grabbing a goal or field traversal,
  //ORIGIN is the presumed starting point of the path,
  //TARGET is the target of the path, such as a goal or general location on the field (e.g. the start position)
  //SIDE is the field side the path is designed for. Not used in skills paths.
  void initSkillsPaths()
  {
    PATHGEN(0.5, 0, 0, "Jaunt_6")
    PATHGEN(0.75, 0, 0, "Jaunt_9")
    PATHGEN(1, 0, 0, "Jaunt_12")
    PATHGEN(0.9, 0, 0, "Grab_Parking_AllianceGoal")
    PATHGEN(3.5, 0, 0, "Rush_Parking_SmallNeutral")
    PATHGEN(4.25, 0, 0, "Traverse_SmallNeutral_ScoringZoneFar")
    PATHGEN(1, 0, 0, "Traverse_ScoringZoneFar_AllianceGoal")
    PATHGEN(4.25, 0, 0, "Traverse_ScoringZoneFar_LargeNeutral")
    PATHGEN(5, 0, 0, "Traverse_LargeNeutral_ScoringZoneNear")
    PATHGEN(3, 0, 0, "Traverse_BalanceNear_AllianceGoal")
    PATHGEN(4, 0, 0, "Traverse_ScoringZoneNear_SmallNeutral")
    PATHGEN(4, 0, 0, "Traverse_SmallNeutral_ScoringZoneFar_V2")
    PATHGEN(2, 0, 0, "Traverse_ScoringZoneFar_RampAllianceGoal") 
    PATHGEN(3, 0, 0, "Attempt_Balance")              
  }

  void initRightSimplePaths()
  {
    PATHGEN(4, 0, 0, "Rush_Parking_SmallNeutral_Right")
    PATHGEN(3, 0, 0, "Return_SmallNeutral_ScoringZoneNear_Right")
    PATHGEN(0.4, 0, 0, "JauntBack")
  }

  void initLeftSimplePaths()
  {
    PATHGEN(4, 0, 0, "Rush_Parking_SmallNeutral_Left")
    PATHGEN(4, 0, 0, "Return_SmallNeutral_ScoringZoneNear_Left")
  }

  void initRightSpinPaths()
  {
    PATHGEN(1.6, 0, 0, "Backgrab_Parking_AllianceGoal_Right")
  }

  void initRightMiddlePaths()
  {
    PATHGEN(5, 0, 0, "Rush_Parking_LargeNeutral_Right")
    PATHGEN(1.9, 0, 0, "Backgrab_Parking_AllianceGoal_Right")
  }

  void initLeftMiddlePaths()
  {
    PATHGEN(1, 0, 0, "Peek_Out")
    PATHGEN(5.5, 0, 0, "Rush_Parking_LargeNeutral_Left")
  }

  //Precomputes Okapi paths used in standard auton.
  void initPaths(int side, int strategy)
  {
      PATHGEN(3, 0, 0, "TestPath")
      if(side == Skills) { initSkillsPaths(); return; }
      
      if(side == Right)
      {
        switch (strategy)
        {
          case SimpleRush:
          {
            initRightSimplePaths();
          }

          case SpinRush:
          {
            initRightSimplePaths();
            initRightSpinPaths();
          }

          case MiddleRush:
          {
            initRightSimplePaths();
            initRightMiddlePaths();
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

          case SpinRush:
          {
            initLeftSimplePaths();
          }

          case MiddleRush:
          {
            initLeftMiddlePaths(); 
          }

          default:
          {
            return;
          }
        }
        return;
      }
  }
