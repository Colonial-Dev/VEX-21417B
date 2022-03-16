#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "Enums.h"
#include "RobotSetup.h"
#include "Display/DisplaySuite.h"
#include "AutonSubroutines.h"
#include "Macros.h"
#include "DriverControl.h"

void initialize() 
{
  //while(inertial_sensor.is_calibrating()) { pros::delay(2); }

  setupBrakeModes();
  splashDisplay();

  tacitPrint("Beginning power-on self test.");
  tacitPrint("Motors [OK]");
  tacitPrint("Pneumatics [OK]");
  tacitPrint("ADI Sensors [OK]");
  tacitPrint("Inertial Sensor . . . [OK]");
  tacitPrint("OkapiLib / Chassis [OK]");
  tacitPrint("OkapiLib / Odometry [OK]");
  tacitPrint("POST OK - Ready!");

  if(autonJumpFlag) { autonomous(); exit(0); }

  if(driverJumpFlag) { opcontrol(); return; }
}

void autonomous()
{
  drive_train->driveToPoint({2_ft, -2_ft});
  drive_train->waitUntilSettled();
  /*if(targetAutonSide == None) { return; }

  if(targetAutonSide == Left)
  {

  }

  else if(targetAutonSide == Right)
  {

  }

  else if(targetAutonSide == Skills)
  {
    
  }*/
}

