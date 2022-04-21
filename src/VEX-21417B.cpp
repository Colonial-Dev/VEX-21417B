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
  //pros::delay(1000);
  //PRINT(rollAngle180(270_deg).convert(radian));
  //PRINT(constrainAngle180(270_deg).convert(radian));
}

void autonomous()
{
  GenerationParameters g_params {15.0, 12_in};
  
  wayfarer.buildPath("Skills_TurnOut", g_params)
    .withPoint({0_ft, 0_ft, -90_deg})
    .withPoint({2_ft, 4_ft, 90_deg})
    .withPoint({2_ft, 8_ft, 90_deg})
    //.withDebugDump()
    .generatePath();
  
  wayfarer.buildPath("STurn", {10, 14_in})
    .withPoint({0_ft, 0_ft, 0_deg})
    .withPoint({6_ft, 0_ft, 0_deg})
    .withDebugDump()
    .generatePath();
  
  //wayfarer.synchronousTraverse("STurn");
  auto traverser = wayfarer.getTraverser("STurn");
  //traverser.simulateStep({0_ft, 0_ft, -45_deg});
  //traverser.simulateStep({1.16_ft, 0.08_ft, 10_deg});
  traverser.simulateStep({0_ft, 0_ft, -90_deg});
  //traverser.simulateStep({4.047851_ft, 3.622648_ft, 125.583423_deg});
  //traverser.simulateStep({4.052967_ft, 3.616032_ft, 127.718597_deg});
}

