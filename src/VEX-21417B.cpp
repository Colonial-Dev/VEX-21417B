#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"

#include "robokauz/Other/DriverControl.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"
#include "robokauz/Autonomous/OdomControllers.hpp"

void initialize() 
{
  overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  initializeDisplay();
}

void autonomous()
{
  GenerationParameters g_params {0.2, 0.8, 0.001, 3};

  wayfarer.buildPath("Alpha", g_params)
      .withLookahead(18_in)
      .withOrigin()
      .withPoint({1.04_ft, 1.47_ft})
      .withPoint({1.1_ft, 4.3_ft})
      .withLookahead(40_in)
      .withPoint({0.86_ft, 4.2_ft})
      .generatePath();

  wayfarer.buildPath("Test", g_params)
      .withLookahead(24_in)
      .withOrigin()
      .withPoint({2_ft, 0_ft})
      .withPoint({4_ft, 2_ft})
      .generatePath();
}

