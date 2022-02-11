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

  brainPrint("#ffff00 [WARN]# Starting auton path precomputation!");
  initPaths(targetAutonSide, targetAutonStrategy);
  brainPrint("Auton paths #00ff00 [OK]#");

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
  if(targetAutonSide == Null) { return; }
  //if(autonTestFlag) { goto test; }

  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == SimpleRush || SpinRush)
    {
      FRONT_CLAMP_OPEN
      PATH("Rush_Parking_SmallNeutral_Left")
      FRONT_CLAMP_CLOSE
      MAIN_LIFT_HOVER
      PATHBACK("Return_SmallNeutral_ScoringZoneNear_Left")
      FRONT_CLAMP_OPEN
      MAIN_LIFT_TARE

      if(targetAutonStrategy == SpinRush)
      {
        TURN(-90)
        REAR_LIFT_UP
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {

    }
    else if(targetAutonStrategy == ComplexRush)
    {

    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == SimpleRush || SpinRush)
    {
      FRONT_CLAMP_OPEN
      PATH("Rush_Parking_SmallNeutral_Right")
      FRONT_CLAMP_CLOSE
      MAIN_LIFT_HOVER
      PATHBACK("Return_SmallNeutral_ScoringZoneNear_Right")
      FRONT_CLAMP_OPEN
      MAIN_LIFT_TARE

      if(targetAutonStrategy == SpinRush)
      {
        TURN(-100)
        PATHBACK("Backgrab_Parking_AllianceGoal_Right")
        REAR_LIFT_UP
        PATH("Backgrab_Parking_AllianceGoal_Right")
      }
    }
    else if(targetAutonStrategy == MiddleRush)
    {
    
    }
    else if(targetAutonStrategy == ComplexRush)
    {
      
    }
  }

  else if(targetAutonSide == Skills)
  {
    THROTTLE(300)
    FRONT_CLAMP_OPEN
    REAR_LIFT_UP

    PATH("Traverse_Hop")
    TURN(-90)
    PATH("Traverse_Hop")
    TURN(180)
    PATHBACK("Traverse_Hop")

    PATH("Rush_Parking_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_ScoringZoneFar")
    TURN(90)
    DROP_FRONT

    TURN(180)
    PATH("Traverse_ScoringZoneFar_AllianceGoal")
    PICKUP_FRONT
    PATHBACK("Traverse_ScoringZoneFar_AllianceGoal")
    DROP_FRONT

    TURN(-135)
    PATH("Traverse_ScoringZoneFar_LargeNeutral")
    PICKUP_FRONT
    PATH("Traverse_LargeNeutral_ScoringZoneNear")
    TURN(135)
    DROP_FRONT

    TURN(180)
    PATH("Traverse_ScoringZoneNear_AllianceGoal")
    PICKUP_FRONT
    PATHBACK("Traverse_ScoringZoneNear_AllianceGoal")
    DROP_FRONT
    TURN(-90)

    PATH("Traverse_ScoringZoneNear_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_ScoringZoneFar")
    TURN(90)
    DROP_FRONT
    TURN(-90)
  }

}

