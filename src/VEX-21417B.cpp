#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"

#include "robokauz/DriverControl.hpp"
#include "robokauz/Autonomous/InertialOdometry.hpp"
#include "robokauz/Autonomous/RoutineMacros.hpp"
#include "robokauz/Autonomous/OdomControllers.hpp"

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

