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
  TURN(90)
  TURN(-90)
  return;
  brainPrint("#0000ff [INFO]# Auton start!");
  if(targetAutonSide == Null) { return; }

  if(targetAutonSide == Left)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == DoubleRush)
    {
      //Rush forwards on slight curve, 
      //grab small neutral and bring it to scoring zone
      //Drop it to the side

      if(targetAutonStrategy == DoubleRush)
      {
        //Curvy path to large neutral and grab it
        //Drag it back to scoring zone
      }
    }
    else if(targetAutonStrategy == StackRush)
    {
      //Rush forwards on slight curve, 
      //grab small neutral and reverse
      //Turn right, path to mid-front of balance
      //Elevate goal, turn it over the balance and drop it
    }
  }

  else if(targetAutonSide == Right)
  {
    if(targetAutonStrategy == SimpleRush || targetAutonStrategy == DoubleRush)
    {
      //Rush forwards, grab small neutral and bring it to scoring zone
      //Drop it to the side

      if(targetAutonStrategy == DoubleRush)
      {
        //Curvy path to large neutral and grab it
        //Drag it back to scoring zone
      }
    }
    else if(targetAutonStrategy == StackRush)
    {
      //Rush forwards on slight curve, 
      //grab small neutral and reverse
      //Turn left, path to mid-front of balance
      //Elevate goal, turn it over the balance and drop it    
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
    //Just... fucking Hail Mary ram the large neutral for a shot at scoring it. (20pts)
    
    //Point total: 160-180pts!! (If I can do it...)
  }

  brainPrint("#0000ff [INFO]# Auton complete!");
}

