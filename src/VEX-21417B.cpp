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

void initialize() 
{
  setupBrakeModes();
  initializeDisplay();
}

void autonomous()
{
  RobotProperties props {1_mps, 0.5_mps2, 11_in, 4.25_in, 0, 0, 0};
  GenerationParameters params {0.2, 0.8, 0.001};
  TraversalParameters t_params {18_in};
  PathManager manager(props);
  manager.generatePath("Test", params, 
                      {{0_ft, 0_ft}, {8_ft, 0_ft}, {8_ft, 8_ft}});
  auto traverser = manager.loadPath("Test", t_params);
  traverser.traversePath();
}

