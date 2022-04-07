#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#define PRINT std::cout <<
#include "Autonomous/InertialOdometry.h"
#include "RobotSetup.h"
#include "RobotManager.h"
#include "Autonomous/RoutineMacros.h"
#include "Autonomous/OdomControllers.h"
#include "Autonomous/PurePursuit/PathManagement.h"
#include "Display/DisplaySuite.h"
#include "DriverControl.h"

RobotProperties global_robot_properties {1_mps, 0.1_mps2, 11.5_in, 4.125_in, drive_train, 0, 0, 0};
PathManager wayfarer(global_robot_properties);

void initialize() 
{
  setupBrakeModes();
  initializeDisplay();
}

void autonomous()
{
  GenerationParameters g_params {0.2, 0.8, 0.001, 1};
  TraversalParameters t_params {12_in};
  wayfarer.generateStandardPath("Test", g_params, 
                      {{0_ft, 0_ft}, {4_ft, 0_ft}, {4_ft, 4_ft}});//, {8_ft, 4_ft}, {8_ft, 8_ft}});
  wayfarer.dumpStoredPath("Test");
  //wayfarer.traverseStoredPath("Test", t_params);
}

