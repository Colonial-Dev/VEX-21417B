#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/CONTROLLERS.hpp"

#include "robokauz/Other/DriverControl.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"
#include <regex>

void sanitize_string(std::string& str)
{
    std::string::iterator iter = str.begin();
    std::string::iterator end = str.end();
    while (iter != end)
    {
        iter = std::find(iter, end, '\b');
        if (iter == end) break;
        if (iter == str.begin())
            iter = str.erase(iter);
        else
            iter = str.erase(iter-1, iter+1);
        end = str.end();
    }
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void awaitCommand()
{
  while(true)
  {
    std::string input;
    std::getline(std::cin, input);
    sanitize_string(input);
    if(!input.compare("overwatch auton"))
    {
      PRINT("Launching!\n");
      pros::Task autn(autonomousAsync);
    }
    pros::delay(2);
  }
}


void initialize() 
{
  PRINT("21417B Competition Suite - v4.0 Worldsbound\n(C) 2021-2022 SomewhereOutInSpace/James Haywood");
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
  initializeOpcontrol();
  pros::Task cmd(awaitCommand);
}

void autonomous()
{
 // overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  wayfarer.buildPath("Skills_1", {1.7, 18_in, 30, 1.0})
    .withGenerationMode(Spline)
    .withOrigin()
    .withPoint({1.3_ft, -1.0_ft})
    .withPoint({5.5_ft, -0.5_ft})
    .withDebugDump()
    .generatePath();
  
  wayfarer.buildPath("Skills_2", {1.5, 16_in, 30, 0.6})
    .withGenerationMode(Spline)
    .withPoint({5.5_ft, -0.5_ft})
    .withPoint({6_ft, 1_ft})
    .withPoint({9.65_ft, 2.2_ft})
    .withDebugDump()
    .generatePath();
  
  wayfarer.buildPath("Skills_3", {1.5, 16_in, 30, 0.5})
    .withGenerationMode(Spline)
    .withPoint({8.5_ft, -2_ft})
    .withPoint({8.5_ft, 1.5_ft})
    .withPoint({5.25_ft, 3.5_ft})
    .generatePath();

  wayfarer.buildPath("Skills_4", {1.5, 16_in, 30, 0.5})
    .withGenerationMode(Spline)
    .withPoint({5.25_ft, 3.5_ft})
    .withPoint({1.5_ft, 2.25_ft})
    .generatePath();
  
  lock_piston.set_value(true);
  SETROT(-90_deg)
  CLIP_CLOSE
  arm_controller.setTargetAsync(0_deg);
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  wayfarer.synchronousTraverse("Skills_1");
  CLAMP_CLOSE
  pros::delay(250);
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  arm_controller.setTargetAsync(138_deg);
  CONVEYOR_ON
  wayfarer.synchronousTraverse("Skills_2");

  CONVEYOR_OFF
  arm_controller.setTarget(67_deg);
  CLAMP_OPEN
  pros::delay(250);

  arm_controller.setTargetAsync(90_deg);
  wayfarer.traverseDistance(-1.25_ft);
  turnRelative(90_deg);
  pros::delay(350);
  CLIP_OPEN
  pros::delay(250);

  arm_controller.setTargetAsync(0_deg);
  wayfarer.traverseDistance(1_ft);
  turnRelative(-90_deg);
  wayfarer.traverseDistance(1.5_ft);
  CLAMP_CLOSE
  pros::delay(250);

  arm_controller.setTargetAsync(20_deg);
  //wayfarer.traverseDistance(-1_ft);
  pros::delay(500);
  turnRelative(0_deg);
  pros::delay(250);
  arm_controller.setTarget(100_deg);
  wayfarer.traverseDistance(1.6_ft);
  arm_controller.setTarget(67_deg);

  CLAMP_OPEN
  pros::delay(250);
  arm_controller.setTargetAsync(100_deg);
  wayfarer.traverseDistance(-1_ft);
  turnRelative(100_deg);
  pros::delay(250);

  arm_controller.setTargetAsync(0_deg);
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  wayfarer.traverseLinear({8.35_ft, -2.2_ft}, true);
  CLIP_CLOSE
  pros::delay(250);
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);

  wayfarer.synchronousTraverse("Skills_3");
  CLAMP_CLOSE
  pros::delay(250);
  arm_controller.setTarget(20_deg);
  turnRelative(180_deg);
  arm_controller.setTarget(140_deg);
  pros::delay(250);
  wayfarer.synchronousTraverse("Skills_4");
  //wayfarer.traverseLinear({2_ft, 2.5_ft}, false);
  arm_controller.setTarget(67_deg);
  CLAMP_OPEN
  pros::delay(250);
}

