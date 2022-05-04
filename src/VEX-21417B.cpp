#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/ROUTINES.hpp"

#include "robokauz/Other/DriverControl.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"
#include <regex>

void sanitize_string(std::string& str)
{
    std::string::iterator iter = str.begin();
    std::string::iterator end = str.end();
    while (iter != end)
    {
        iter = std::find(iter, end, '\b');
        if (iter == end) break;
        if (iter == str.begin())
            iter = str.erase(iter);
        else
            iter = str.erase(iter-1, iter+1);
        end = str.end();
    }
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void awaitCommand()
{
  while(true)
  {
    std::string input;
    std::getline(std::cin, input);
    sanitize_string(input);
    if(!input.compare("overwatch auton"))
    {
      PRINT("Launching!\n");
      pros::Task autn(autonomousAsync);
    }
    pros::delay(2);
  }
}

void initialize() 
{
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
  initializeOpcontrol();
  pros::Task cmd(awaitCommand);

  //Dirty, unverified way of preventing the OdomChassisController from eating up my fucking processor cycles
  //I would replace it with a regular chassis controller or even a custom one, but that causes regressions I don't have time to debug, soo
  drive_controller->getOdomThread()->notifyTake(TIMEOUT_MAX);
}

void autonomous()
{
  driver_control_gate.closeGate();

  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);

  selectRoutine(targetAutonSide, targetAutonStrategy);
  
  driver_control_gate.openGate();
}

