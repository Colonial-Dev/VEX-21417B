#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor FrontLeft = motor(PORT12, ratio18_1, true);
motor FrontRight = motor(PORT20, ratio18_1, false);
motor BackLeft = motor(PORT1, ratio18_1, true);
motor BackRight = motor(PORT10, ratio18_1, false);
motor LiftDriversMotorA = motor(PORT2, ratio18_1, false);
motor LiftDriversMotorB = motor(PORT9, ratio18_1, true);
motor_group LiftDrivers = motor_group(LiftDriversMotorA, LiftDriversMotorB);
inertial Inertial = inertial(PORT15);
distance North = distance(PORT16);
distance South = distance(PORT17);
distance East = distance(PORT18);
distance West = distance(PORT19);
controller Controller1 = controller(primary);

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