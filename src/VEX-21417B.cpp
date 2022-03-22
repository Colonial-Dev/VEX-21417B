#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "Enums.h"
#include "Macros.h"
#include "RobotSetup.h"
#include "Autonomous/Subroutines.h"
#include "Display/DisplaySuite.h"
#include "DriverControl.h" 

void initialize() 
{
  //while(inertial_sensor.is_calibrating()) { pros::delay(2); }
  setupBrakeModes();
  splashDisplay();
}

void autonomous()
{
  drive_train->turnToAngle(90_deg);
  drive_train->waitUntilSettled();
  drive_train->turnToAngle(0_deg);
  drive_train->waitUntilSettled();

  /*drive_train->driveToPoint({2_ft, 0_ft});
  drive_train->waitUntilSettled();
  drive_train->driveToPoint({2_ft, 2_ft});
  drive_train->waitUntilSettled();
  drive_train->driveToPoint({0_ft, 2_ft});
  drive_train->waitUntilSettled();
  drive_train->driveToPoint({0_ft, 0_ft});
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

