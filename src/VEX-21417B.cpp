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
  overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
  initializeOpcontrol();
  pros::Task cmd(awaitCommand);
}

void autonomous()
{
  wayfarer.buildPath("LineTest", {2.0, 18_in})
    .withGenerationMode(Rough)
    .withOrigin()
    .withPoint({6_ft, 6_ft})
    .generatePath();

  wayfarer.buildPath("CurveTest", {2.0, 20_in})
    .withGenerationMode(ConstrainedSpline)
    .withOrigin()
    .withPoint({6_ft, 6_ft, 90_deg})
    .generatePath();

  wayfarer.buildPath("Skills_1", {1.7, 18_in, 30, 1.0})
    .withGenerationMode(Spline)
    .withOrigin()
    .withPoint({1.0_ft, 1.3_ft})
    .withPoint({0.5_ft, 5.5_ft})
    .withDebugDump()
    .generatePath();
  
  wayfarer.buildPath("Skills_2", {1.5, 16_in, 30, 0.6})
    .withGenerationMode(Spline)
    .withPoint({0.5_ft, 5.5_ft})
    .withPoint({-1_ft, 6_ft})
    .withPoint({-2.25_ft, 9.65_ft})
    .withDebugDump()
    .generatePath();
  
  CLIP_CLOSE
  arm_controller.setTargetAsync(0_deg);
  wayfarer.synchronousTraverse("Skills_1");
  CLAMP_CLOSE
  pros::delay(500);
  arm_controller.setTargetAsync(138_deg);
  CONVEYOR_ON
  wayfarer.synchronousTraverse("Skills_2");
  turnRelative(90_deg);
  CONVEYOR_OFF
  arm_controller.setTarget(70_deg);
  CLAMP_OPEN
  
  /*arm_controller.setTarget(0_deg);
  CLIP_CLOSE
  wayfarer.synchronousTraverse("Skills_1");
  CLAMP_CLOSE
  pros::delay(500);
  arm_controller.setTarget(138_deg);
  CONVEYOR_ON
  wayfarer.synchronousTraverse("Skills_2");
  CONVEYOR_OFF
  arm_controller.setTarget(120_deg);
  CLAMP_OPEN*/
}

