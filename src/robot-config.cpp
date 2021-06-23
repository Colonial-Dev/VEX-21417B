#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FL = motor(PORT3, ratio6_1, true);
motor FR = motor(PORT10, ratio6_1, false);
motor BL = motor(PORT11, ratio6_1, true);
motor BR = motor(PORT20, ratio6_1, false);
controller Controller1 = controller(primary);
bumper BumperA = bumper(Brain.ThreeWirePort.A);

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