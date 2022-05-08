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

void initialize() 
{
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
  initializeOpcontrol();
  conveyor_controller.disableUnjam();
}

void disabled()
{
  driver_control_gate.openGate();
  conveyor_controller.disableUnjam();
  wayfarer.forceAbort();
  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);
}

void autonomous()
{
  driver_control_gate.closeGate();
  conveyor_controller.enableUnjam();

  overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);

  selectRoutine(targetAutonSide, targetAutonStrategy);
  
  conveyor_controller.disableUnjam();
  driver_control_gate.openGate();
}

