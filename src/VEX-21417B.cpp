#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/DISPLAY.hpp"
#include "robokauz/PURE_PURSUIT.hpp"

#include "robokauz/DriverControl.hpp"
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
  TraversalParameters t_params {12_in};

  wayfarer.generateStandardPath("Skills_1", g_params,
                                {{0_ft, 0_ft}, {1.04_ft, 1.47_ft}});//, {1.22_ft, 2.28_ft}, {0.86_ft, 4.2_ft}});

  wayfarer.generateStandardPath("Skills_2", g_params,
                                {{1.04_ft, 1.47_ft}, {1.1_ft, 4.3_ft}});

  wayfarer.generateStandardPath("Skills_3", g_params,
                                {{1.22_ft, 2.28_ft}, {0.86_ft, 4.2_ft}});
  
  CLIP_CLOSE
  wayfarer.traverseStoredPath("Skills_1", t_params);
  //drive_train->turnToPoint({0.59_ft, 3.91_ft});
  wayfarer.traverseStoredPath("Skills_2", {30_in});
  //wayfarer.traverseStoredPath("Skills_3", {40_in});
  CLAMP_CLOSE
  pros::delay(1000);

}

