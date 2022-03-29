#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "RobotManager.h"
#include "Autonomous/RateLimiter.h"
#include "RobotSetup.h"
#include "Autonomous/RoutineMacros.h"
#include "Autonomous/Subroutines.h"
#include "Autonomous/OdomController.h"
#include "Autonomous/PathManagement.h"
#include "Display/DisplaySuite.h"
#include "DriverControl.h" 

void initialize() 
{
  setupBrakeModes();
  initializeDisplay();
}

void autonomous()
{

}

