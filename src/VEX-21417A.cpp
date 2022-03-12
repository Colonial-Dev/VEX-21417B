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
    //Grab enemy line goal and angle-ram small neutral to enemy zone
    FRONT_CLAMP_OPEN
    PATH("Rush_Start_AllianceGoal")
    PICKUP_FRONT
    TURN(-90)
    PATH("Traverse_AllianceGoal_RamStart")
    TURN(20)
    PATH("Ram_SmallNeutral_EnemyZone")
    DROP_FRONT
    PATHBACK("Jaunt_12")

    //Grab friendly goal from balance
    TURN(-30)
    PATH("Jaunt_24")
    PICKUP_FRONT
    MAIN_LIFT_BALANCE
    TURN(-180)
    MAIN_LIFT_HOVER

    //Ram large neutral to friendly zone
    PATH("Jaunt_18")
    TURN(-140)
    PATH("Ram_EnemyZone_LargeNeutral")
    DROP_FRONT

    //Grab enemy goal from balance
    TURN(-230)
    PATH("Jaunt_18")
    PICKUP_FRONT
    MAIN_LIFT_BALANCE
    TURN(0)
    MAIN_LIFT_HOVER

    //Ram small neutral to enemy zone
    PATH("Ram_FriendlyZone_SmallNeutral")
    DROP_FRONT

    //Grab friendly line goal
    TURN(-90)
    PATH("Jaunt_24")
    FRONT_CLAMP_CLOSE
    PATHBACK("Jaunt_18")
    MAIN_LIFT_HOVER

    //Drop friendly goal in friendly zone
    TURN(-180)
    PATH("Traverse_EnemyZone_FriendlyZone")
    DROP_FRONT
  }

  brainPrint("#0000ff [INFO]# Auton complete!");
}

