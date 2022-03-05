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

void inertialTurn(double angle)
{
	double error = angle - inertial_sensor.get_rotation();
  double threshold = 0.93 + (0.0025 * fabs(error)) + fabs(error * 0.001);
	double integral;
	double derivative;
	double prevError;
	double kp = 1.2;
	double ki = 0.0025;
	double kd = 7.5;

	while(fabs(error) > threshold)
  {
		error = angle - inertial_sensor.get_rotation();
    debugPrint(std::to_string(error) + " || " + std::to_string(right_back.get_actual_velocity()) + " || " + to_string(threshold));
    integral  = integral + error;

		if(error == 0 || fabs(error) >= angle)
    {
			integral = 0;
		}

		derivative = error - prevError;
		prevError = error;
		double p = error * kp;
		double i = integral * ki;
		double d = derivative * kd;

		double vel = p + i + d;

		right_back.move_velocity(-vel);
    right_middle.move_velocity(-vel);
    right_front.move_velocity(-vel);

    left_back.move_velocity(vel);
    left_middle.move_velocity(vel);
    left_front.move_velocity(vel);

		pros::delay(15);
	}
  right_back.move_velocity(0);
  right_middle.move_velocity(0);
  right_front.move_velocity(0);

  left_back.move_velocity(0);
  left_middle.move_velocity(0);
  left_front.move_velocity(0);
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
    TURN(-50)
    PATH("Jaunt_18")
    PICKUP_FRONT
    MAIN_LIFT_BALANCE
    TURN(-180)
    MAIN_LIFT_HOVER

    //Ram large neutral to friendly zone
    PATH("Jaunt_18")
    TURN(-120)
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
    PATH("Jaunt_18")
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

