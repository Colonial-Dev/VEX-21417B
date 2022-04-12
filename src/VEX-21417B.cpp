#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"

#include "robokauz/DriverControl.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"
#include "robokauz/Autonomous/OdomControllers.hpp"

void initialize() 
{
  overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
}

void autonomous()
{
  GenerationParameters g_params {0.2, 0.8, 0.001, 2};
  TraversalParameters t_params {24_in};
  wayfarer.generateStandardPath("Test", g_params, 
                      {{0_ft, 0_ft}, {2_ft, 0_ft}, {4_ft, 2_ft}});//, {8_ft, 4_ft}, {8_ft, 8_ft}});
  wayfarer.dumpStoredPath("Test");
  wayfarer.traverseStoredPath("Test", t_params);
}

