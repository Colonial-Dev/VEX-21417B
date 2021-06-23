using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor FL;
extern motor FR;
extern motor BL;
extern motor BR;
extern controller Controller1;
extern bumper BumperA;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );