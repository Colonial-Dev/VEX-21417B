#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "Enums.h"
#include "InterfaceSetup.h"
#include "RobotSetup.h"
#include "Macros.h"
#include "DisplaySuite.h"
#include "DriverControl.h"

void initialize() 
{
  setupBrakeModes();
  splashDisplay();
  manager.Update(levels);
  brainPrint("Menu system backend #00ff00 [OK]#");

  brainPrint("Broadcasting menu system interface...");
  advanced_auton_select(master);

  initPaths(targetAutonSide, targetAutonStrategy);
  brainPrint("Auton paths #00ff00 [OK]# (Target: " + targetAutonSideLabel + "_" + targetAutonStrategyLabel + ", Total: " + std::to_string(computedPaths) + ")");

  if(autonTestFlag) { autonomous(); exit(0); }

	pros::Task holdMessagePrinting(holdCycle);
  while(true)
  {
      if(master.get_digital_new_press(DIGITAL_DOWN)){break;}
      if(pros::competition::is_connected()){break;}
      pros::delay(2);
  }

  holdMessagePrinting.remove();
  return;
}

void autonomous()
{
  brainPrint("#0000ff [INFO]# Auton start!");
  if(targetAutonSide == Null) { return; }

  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == SpinRush)
    {
      FRONT_CLAMP_OPEN
      //REAR_LIFT_DOWN
      PATHMIRROR("Rush_Parking_SmallNeutral_Left")
      PICKUP_FRONT
      PATHBACKMIRROR("Rush_Parking_SmallNeutral_Left")
      DROP_FRONT
      TURN(-90)

      if(targetAutonStrategy == SpinRush)
      {
        PATHBACK("Backgrab_Parking_AllianceGoal_Left")
        //REAR_LIFT_UP
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      FRONT_CLAMP_OPEN
      //REAR_LIFT_DOWN
      PATHMIRROR("Rush_Parking_LargeNeutral_Left")
      PICKUP_FRONT
      PATHBACKMIRROR("Rush_Parking_LargeNeutral_Left")
      DROP_FRONT           
      TURN(-90)
      PATHBACK("Backgrab_Parking_AllianceGoal_Left")
      //REAR_LIFT_UP
    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == SpinRush)
    {
      FRONT_CLAMP_OPEN
      //REAR_LIFT_DOWN
      PATH("Rush_Parking_SmallNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Return_SmallNeutral_ScoringZoneNear_Right")
      DROP_FRONT
      PATHBACK("JauntBack")

      if(targetAutonStrategy == SpinRush)
      {
        TURN(-120)
        PATHBACK("Backgrab_Parking_AllianceGoal_Right")
        //REAR_LIFT_UP
        PATH("Backgrab_Parking_AllianceGoal_Right")
        TURN(120)
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      FRONT_CLAMP_OPEN
      //REAR_LIFT_DOWN
      PATH("Rush_Parking_LargeNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Rush_Parking_LargeNeutral_Right")
      DROP_FRONT
      PATHBACK("JauntBack")
      TURN(-120)
      PATHBACK("Backgrab_Parking_AllianceGoal_Right")
      //REAR_LIFT_UP
      PATH("Backgrab_Parking_AllianceGoal_Right")
      TURN(120)
    }
  }

  else if(targetAutonSide == Skills)
  {
    
  }
  brainPrint("#0000ff [INFO]# Auton complete!");
}

