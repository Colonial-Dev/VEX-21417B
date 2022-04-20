#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"

#include "robokauz/Other/DriverControl.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"
#include "robokauz/Autonomous/OdomControllers.hpp"
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
      pros::Task autn(autonomousAsync);
    }
    pros::delay(2);
  }
}


void initialize() 
{
  overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
  pros::Task cmd(awaitCommand);
  inertial_sensor.set_rotation(-90);
}

void autonomous()
{
  GenerationParameters g_params {3, 12_in};

  CLIP_CLOSE

  wayfarer.buildPath("WinPoint_TurnOut", g_params)
    .withRobotProperties({0.3_mps, 0.1_mps2, 11.5_in, 4.125_in, drive_train})
    .withPoint({0_ft, 0_ft, -90_deg})
    .withPoint({0_ft, -0.9_ft, -45_deg})
    .withPoint({1.5_ft, -0.9_ft, 0_deg})
    //.withPoint({3_ft, 0_ft})
    //.withPoint({3_ft, 1_ft})
    //.withPoint({3_ft, 3_ft})
    //.withPoint({2_ft, 3_ft})
    .withDebugDump()
    .generatePath();
  
  wayfarer.buildPath("WinPoint_Long", g_params)
    .withPoint({3_ft, 1_ft, 90_deg})
    .withPoint({3.5_ft, 5.25_ft, 90_deg})
    .generatePath();

  wayfarer.traverseStoredPath("WinPoint_TurnOut");
  //pros::delay(5000);
  //wayfarer.traverseStoredPath("WinPoint_Long");

  //turnRelative(-70_deg);

  /*wayfarer.buildPath("Alpha", g_params)
      .withLookahead(18_in)
      .withOrigin()
      .withPoint({1.04_ft, 1.47_ft})
      .withLookahead(40_in)
      .withPoint({1.1_ft, 4.3_ft})
      .withPoint({0.86_ft, 4.2_ft})
      .withDebugDump()
      .generatePath();

  wayfarer.buildPath("Straight", g_params)
    .withLookahead(6_in)
    .withOrigin()
    .withPoint({4_ft, 0_ft})
    .makeReversed()
    .generatePath();

  wayfarer.buildPath("Test", g_params)
      .withLookahead(24_in)
      .withOrigin()
      .withPoint({3_ft, 0_ft})
      .withPoint({6_ft, 4_ft})
      .makeReversed()
      .withDebugDump()
      .generatePath();
  
  wayfarer.buildPath("TestReturn", g_params)
    .withLookahead(24_in)
    .withPoint({6_ft, 4_ft})
    .withOrigin()
    .generatePath();*/


  //wayfarer.traverseStoredPath("Straight");
  //wayfarer.traverseStoredPath("Straight_rev");
  
  /*wayfarer.traverseStoredPath("Test");
  drive_train->turnToPoint({0_ft, 0_ft});
  drive_train->waitUntilSettled();
  wayfarer.traverseStoredPath("TestReturn");*/
}

