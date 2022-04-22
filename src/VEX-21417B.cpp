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
      PRINT("Launching!");
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
  //inertial_sensor.set_rotation(-90);
}

void autonomous()
{
  GenerationParameters g_params {15.0, 12_in};
  
  /*wayfarer.buildPath("Skills_TurnOut", g_params)
    .withPoint({0_ft, 0_ft, -90_deg})
    .withPoint({2_ft, 4_ft, 90_deg})
    .withPoint({2_ft, 8_ft, 90_deg})
    //.withDebugDump()
    .generatePath();*/

  /*
    path_finder->generatePath({
    {1.216_m, 0.608_m, 270_deg},
    {1.824_m, 1.815_m, 180_deg},
    {1.824_m, 3.040_m, 180_deg}},
    "Test"
  );*/

  wayfarer.buildPath("Lines", {1, 12_in})
    .setGenerationMode(Rough)
    .withOrigin()
    .withPoint({4_ft, 4_ft, 0_deg})
    .generatePath();
  
  wayfarer.buildPath("Corners", {1, 12_in})
    .setGenerationMode(Rough)
    .withPoint({0_ft, 0_ft, 0_deg})
    .withPoint({3_ft, 0_ft, 0_deg})
    .withPoint({3_ft, 3_ft, 0_deg})
    .withPoint({3_ft, 6_ft, 0_deg})
    .withPoint({6_ft, 6_ft, 0_deg})
    //.withDebugDump()
    .generatePath();

  wayfarer.buildPath("Curves", {2, 12_in})
    .setGenerationMode(Spline)
    .withOrigin()
    .withPoint({4_ft, 4_ft, 90_deg})
    .withDebugDump()
    .generatePath();
  
  //wayfarer.synchronousTraverse("Lines");
  auto traverser = wayfarer.getTraverser("Lines");
  traverser.simulateStep({0_ft, 0_ft, 0_deg});
  traverser.simulateStep({2_ft, 2_ft, 45_deg});
  traverser.simulateStep({1.141246_ft, 2.645605_ft, 45.602719_deg});
  //traverser.simulateStep({3_ft, 0_ft, 90_deg});
}

