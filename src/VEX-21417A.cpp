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
  //if(autonTestFlag) { goto test; }

  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == SpinRush)
    {
      FRONT_CLAMP_OPEN
      REAR_LIFT_DOWN
      PATH("Rush_Parking_SmallNeutral_Left")
      FRONT_CLAMP_CLOSE
      MAIN_LIFT_HOVER
      PATHBACK("Rush_Parking_SmallNeutral_Left")
      FRONT_CLAMP_OPEN
      MAIN_LIFT_TARE
      TURN(-90)

      if(targetAutonStrategy == SpinRush)
      {
        REAR_LIFT_UP
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
      FRONT_CLAMP_OPEN
      REAR_LIFT_DOWN
      PATH("Peek_Out")
      TURN(45)
      PATH("Rush_Parking_LargeNeutral_Left")
      PICKUP_FRONT
      PATHBACK("Rush_Parking_LargeNeutral_Left")
      TURN(-45)
      DROP_FRONT           
      PATHBACK("Peek_Out")
      TURN(-90)
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
      TURN(-35)
      PATH("Rush_Parking_LargeNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Rush_Parking_LargeNeutral_Right")
      TURN(35)
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
    //Grab alliance goal and do fancy footwork to safely relocate it to the back lift
    REAR_LIFT_DOWN
    FRONT_CLAMP_OPEN
    PATH("Grab_Parking_AllianceGoal")
    PICKUP_FRONT
    MAIN_LIFT_CUSTOM(1900)
    PATHBACK("Grab_Parking_AllianceGoal")
    TURN(-100)
    MAIN_LIFT_HOVER
    PATH("Jaunt_12")
    DROP_FRONT
    PATHBACK("Jaunt_9")
    //Note: turn accuracy and PID wiggle become worse with larger angles,
    //so it's best to do things in max 90deg increments even if some speed is lost.
    TURN(-90)
    TURN(-90)
    PATHBACK("Jaunt_12")
    REAR_LIFT_UP
    TURN(-90)
    TURN(-90)

    //Grab small neutral and place it in opposite scoring zone
    PATH("Rush_Parking_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_ScoringZoneFar")
    DROP_FRONT
    PATHBACK("Jaunt_9")
    TURN(-90)

    //Grab alliance goal and drag it out into scoring zone
    PATH("Traverse_ScoringZoneFar_AllianceGoal")
    PICKUP_FRONT
    PATHBACK("Traverse_ScoringZoneFar_AllianceGoal")
    DROP_FRONT

    //Grab large neutral and dump it in scoring zone
    TURN(-90)
    TURN(-45)
    PATH("Traverse_ScoringZoneFar_LargeNeutral")
    PICKUP_FRONT
    MAIN_LIFT_BALANCE
    PATH("Traverse_LargeNeutral_ScoringZoneNear")
    TURN(45)
    DROP_FRONT
    TURN(-90)

    //Grab alliance goal, drag it out into scoring zone
    PATH("Traverse_BalanceNear_AllianceGoal")
    PICKUP_FRONT
    PATHBACK("Jaunt_12")
    DROP_FRONT
    TURN(90)
    TURN(-90)
    TURN(-90)

    //Path to and grab remaining small neutral and drop it in scoring zone
    PATH("Traverse_ScoringZoneNear_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_ScoringZoneFar_V2")
    TURN(90)
    DROP_FRONT
    TURN(-90)

    //Move to balance, grab last alliance goal and try (TM) to balance
    PATH("Traverse_ScoringZoneFar_RampAllianceGoal")
    TURN(-90)
    PICKUP_FRONT
    PATH("Attempt_Balance")
  }
  brainPrint("#0000ff [INFO]# Auton complete!");
}

