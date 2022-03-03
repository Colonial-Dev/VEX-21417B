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

  if(opJumpFlag) { opcontrol(); return; }

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
    FRONT_CLAMP_OPEN
    PATHMIRROR("Rush_Parking_SmallNeutral_Left")
    PICKUP_FRONT
    PATHBACKMIRROR("Rush_Parking_SmallNeutral_Left")

    if(targetAutonStrategy == DoubleRush)
    {
      TURN(-45)
      DROP_FRONT
      TURN(45)

      FRONT_CLAMP_OPEN
      PATHMIRROR("Rush_Parking_LargeNeutral_Left")
      PICKUP_FRONT
      PATHBACKMIRROR("Rush_Parking_LargeNeutral_Left")

      TURN(45)
      DROP_FRONT
      TURN(-45)
    }

    if(targetAutonStrategy == StackRush)
    {
      PATH("Peek_Out")
      TURN(90)
      PATH("Traverse_Parking_Balance_Left")
      MAIN_LIFT_BALANCE
      TURN(90)
      FRONT_CLAMP_OPEN
    }
  }

  else if(targetAutonSide == Right)
  {
    FRONT_CLAMP_OPEN
    PATH("Rush_Parking_SmallNeutral_Right")
    PICKUP_FRONT
    PATHBACK("Rush_Parking_SmallNeutral_Right")

    if(targetAutonStrategy == DoubleRush)
    {
      TURN(45)
      DROP_FRONT
      TURN(-45)

      FRONT_CLAMP_OPEN
      PATH("Rush_Parking_LargeNeutral_Right")
      PICKUP_FRONT
      PATHBACK("Rush_Parking_LargeNeutral_Right")

      TURN(-45)
      DROP_FRONT
      TURN(45)
    }

    if(targetAutonStrategy == StackRush)
    {
      PATH("Peek_Out")
      TURN(-90)
      PATH("Traverse_Parking_Balance_Right")
      MAIN_LIFT_BALANCE
      TURN(-90)
      FRONT_CLAMP_OPEN
    }
  }

  else if(targetAutonSide == Skills)
  {
    //Start on the RIGHT
    //Rush forwards, acquire small neutral and continue to far side
    //Balance small neutral on enemy side (40pts)
    //Retrace steps, grabbing the friendly alliance goal that was on the balance
    //Dump friendly alliance goal and grab enemy one (20pts)
    //Return to far side and dump enemy alliance goal (20pts)
    //Head south to grab second small neutral, and balance it on friendly side (40pts)
    //Retrace steps, grabbing enemy alliance goal that was on the balance
    //Dump enemy alliance goal and grab friendly one (20pts)
    //Return to near side and dump friendly alliance goal (20pts)
    //Just... fucking Hail Mary ram the large neutral for a shot at scoring it. (~20pts)
    
    //Point total: 160-180pts!! (If I can do it...)
        
    //driveTrain->setMaxVelocity(80);
    FRONT_CLAMP_OPEN
    PATH("Rush_Start_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_EnemyZone")

    TURN(-90)
    PATH("Traverse_EnemyZone_EnemyBalance")
    MAIN_LIFT_MAX
    TURN(90)
    PATH("Contact_Balance")
    MAIN_LIFT_BALANCE
    FRONT_CLAMP_OPEN
    MAIN_LIFT_MAX
    PATHBACK("Contact_Balance")
    TURN(90)
    MAIN_LIFT_TARE

    PATH("Traverse_EnemyBalance_EnemyZone")
    TURN(-90)
    TURN(-45)
    PATH("Grab_RampAllianceGoal")
    PICKUP_FRONT
    PATHBACK("Grab_RampAllianceGoal")
    TURN(-45)

    TURN(-90)
    PATH("Traverse_EnemyZone_FriendlyZone")
    DROP_FRONT

    TURN(-90) 
    PATH("Grab_LineAllianceGoal")
    PICKUP_FRONT
    PATHBACK("Grab_LineAllianceGoal")
    TURN(-90)

    PATH("Traverse_FriendlyZone_EnemyZone")
    DROP_FRONT
    PATHBACK("Jaunt_12")
    TURN(-90)
    PATH("Traverse_EnemyZoneSouth_EnemyZoneNorth")
    TURN(-90)

    //Rinse and Repeat

    PATH("Rush_EnemyZone_SmallNeutral")
    PICKUP_FRONT
    PATH("Traverse_SmallNeutral_FriendlyZone")

    TURN(-90)
    PATH("Traverse_FriendlyZone_FriendlyBalance")
    MAIN_LIFT_BALANCE
    TURN(90)
    PATH("Contact_Balance")
    FRONT_CLAMP_OPEN
    PATHBACK("Contact_Balance")
    TURN(90)
    MAIN_LIFT_TARE

    PATH("Traverse_FriendlyBalance_FriendlyZone")
    TURN(-90)
    TURN(-45)
    PATH("Grab_RampAllianceGoal")
    PICKUP_FRONT
    PATHBACK("Grab_RampAllianceGoal")
    TURN(-45)

    TURN(-90)
    PATH("Traverse_EnemyZone_FriendlyZone")
    DROP_FRONT

    TURN(-90) 
    PATH("Grab_LineAllianceGoal")
    PICKUP_FRONT
    PATHBACK("Grab_LineAllianceGoal")
    TURN(-90)

    PATH("Traverse_FriendlyZone_EnemyZone")
    DROP_FRONT
    PATHBACK("Jaunt_12")
    TURN(-90)
    TURN(-45)
    PATH("Hail_Mary")
  }

  brainPrint("#0000ff [INFO]# Auton complete!");
}

