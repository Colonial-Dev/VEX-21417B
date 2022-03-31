#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "RobotManager.h"
#include "Autonomous/PurePursuit/RateLimiter.h"
#include "RobotSetup.h"
#include "Autonomous/RoutineMacros.h"
#include "Autonomous/Subroutines.h"
#include "Autonomous/OdomControllers.h"
#include "Autonomous/PurePursuit/PathManagement.h"
#include "Display/DisplaySuite.h"
#include "DriverControl.h"

RobotProperties global_robot_properties {1_mps, 0.5_mps2, 11_in, 4.25_in, drive_train, 0, 0, 0};
PathManager wayfarer(global_robot_properties);

void initialize() 
{
  setupBrakeModes();
  initializeDisplay();
}

void autonomous()
{
  GenerationParameters g_params {0.2, 0.8, 0.001, 2.5};
  TraversalParameters t_params {18_in};
  wayfarer.generatePath("Test", g_params, 
                      {{0_ft, 0_ft}, {8_ft, 0_ft}, {8_ft, 8_ft}});
  wayfarer.dumpPath("Test");
  pros::delay(5000);
  wayfarer.traversePath("Test", t_params);
}

