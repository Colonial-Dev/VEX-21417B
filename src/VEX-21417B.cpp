#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "Enums.h"
#include "InterfaceSetup.h"
#include "RobotSetup.h"
#include "DisplaySuite.h"
#include "AutonSubroutines.h"
#include "Macros.h"
#include "DriverControl.h"

void initialize() 
{
  while(inertial_sensor.is_calibrating()) { pros::delay(2); }

  setupBrakeModes();
  splashDisplay();

  if(autonJumpFlag) { autonomous(); exit(0); }

  if(driverJumpFlag) { opcontrol(); return; }
}

void autonomous()
{
  brainPrint("#0000ff [INFO]# Auton start!");
  if(targetAutonSide == Null) { return; }

  if(targetAutonSide == Left)
  {

  }

  else if(targetAutonSide == Right)
  {

  }

  else if(targetAutonSide == Skills)
  {
    
  }

  brainPrint("#0000ff [INFO]# Auton complete!");
}

