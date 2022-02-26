#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include <string>

using namespace std;
using namespace okapi;

#include "Enums.h"
#include "InterfaceSetup.h"
#include "AutonomousSetup.h"
#include "Macros.h"
#include "DisplaySuite.h"
#include "DriverControl.h"

void initialize() 
{
  splashDisplay();
  manager.Update(levels);
  brainPrint("Menu system backend #00ff00 [OK]#");

  pros::Controller master(pros::E_CONTROLLER_MASTER);
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
      REAR_LIFT_DOWN
      PATHMIRROR("Rush_Parking_SmallNeutral_Left")
      PICKUP_FRONT
      PATHBACKMIRROR("Rush_Parking_SmallNeutral_Left")
      DROP_FRONT
      TURN(-90)

      if(targetAutonStrategy == SpinRush)
      {
        PATHBACK("Backgrab_Parking_AllianceGoal_Left")
        REAR_LIFT_UP
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      FRONT_CLAMP_OPEN
      REAR_LIFT_DOWN
      PATHMIRROR("Rush_Parking_LargeNeutral_Left")
      PICKUP_FRONT
      PATHBACKMIRROR("Rush_Parking_LargeNeutral_Left")
      DROP_FRONT           
      TURN(-90)
      PATHBACK("Backgrab_Parking_AllianceGoal_Left")
      REAR_LIFT_UP
    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == SpinRush)
    {
      FRONT_CLAMP_OPEN
      REAR_LIFT_DOWN
      PATH("Rush_Parking_SmallNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Return_SmallNeutral_ScoringZoneNear_Right")
      DROP_FRONT
      PATHBACK("JauntBack")

      if(targetAutonStrategy == SpinRush)
      {
        TURN(-120)
        PATHBACK("Backgrab_Parking_AllianceGoal_Right")
        REAR_LIFT_UP
        PATH("Backgrab_Parking_AllianceGoal_Right")
        TURN(120)
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      FRONT_CLAMP_OPEN
      REAR_LIFT_DOWN
      PATH("Rush_Parking_LargeNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Rush_Parking_LargeNeutral_Right")
      DROP_FRONT
      PATHBACK("JauntBack")
      TURN(-120)
      PATHBACK("Backgrab_Parking_AllianceGoal_Right")
      REAR_LIFT_UP
      PATH("Backgrab_Parking_AllianceGoal_Right")
      TURN(120)
    }
  }

  else if(targetAutonSide == Skills)
  {
    //Grab alliance goal
    REAR_LIFT_DOWN
    PATHBACK("Grab_Parking_AllianceGoal")
    REAR_LIFT_UP

    //Skirt out into open space
    PATHMIRROR("TestPath_MG")
    PATHBACKMIRROR("TestPath_MG")
    TURN(90)
    FRONT_CLAMP_OPEN

    //Grab small neutral and dump it in far zone
    PATH("Rush_Parking_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_ScoringZoneFar")
    DROP_FRONT

    //Dump first alliance goal in a safe spot
    //TODO fucking figure this out geometrically
    PATHBACK("Jaunt_9")
    TURN(90)
    PATHBACK("Jaunt_12")
    REAR_LIFT_DOWN
    PATH("Jaunt_12")
    TURN(-90)
    TURN(-90)

    //Grab second alliance goal
    PATHBACK("Traverse_ScoringZoneFar_AllianceGoal")
    REAR_LIFT_UP
    PATH("Traverse_ScoringZoneFar_AllianceGoal")

    //Head to the lower half of the far zone
    //Then grab the small neutral and drag it back
    PATH("Traverse_ScoringZoneFar_South")
    TURN(90)
    PATH("Traverse_ScoringZoneFar_SmallNeutral")
    PICKUP_FRONT
    PATHBACK("Traverse_ScoringZoneFar_SmallNeutral")
    TURN(90)
    DROP_FRONT
    TURN(-90)

    //Cross to the opposite zone and drop second alliance goal
    PATH("Traverse_ScoringZoneFar_ScoringZoneNear")
    TURN(-90)
    PATHBACK("Jaunt_24")
    REAR_LIFT_DOWN
    PATH("Jaunt_24")

    //Pick up third alliance goal
    TURN(-90)
    TURN(-90)
    PATHBACK("Traverse_ScoringZoneNear_AllianceGoal")
    REAR_LIFT_UP
    PATH("Traverse_ScoringZoneNear_AllianceGoal")
    TURN(45)

    //Pickup large neutral and drop it in the far zone
    PATH("Traverse_ScoringZoneNear_LargeNeutral")
    PICKUP_FRONT
    PATH("Traverse_LargeNeutral_ScoringZoneFar")
    DROP_FRONT
    
    //Drop third alliance goal
    TURN(-45)
    REAR_LIFT_DOWN
  }
  brainPrint("#0000ff [INFO]# Auton complete!");
}

