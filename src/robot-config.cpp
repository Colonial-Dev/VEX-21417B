#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FrontLeft = motor(PORT1, ratio18_1, true);
motor FrontRight = motor(PORT16, ratio18_1, false);
motor BackLeft = motor(PORT9, ratio18_1, true);
motor BackRight = motor(PORT19, ratio18_1, false);
controller Controller1 = controller(primary);
motor FwdLiftMotor = motor(PORT13, ratio18_1, false);
motor LeftLiftMotor = motor(PORT11, ratio36_1, false);
motor RightLiftMotor = motor(PORT12, ratio36_1, false);
motor RearLiftMotor = motor(PORT15, ratio36_1, false);
inertial Inertial = inertial(PORT5);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}