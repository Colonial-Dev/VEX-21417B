/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// FL                   motor         3               
// FR                   motor         10              
// BL                   motor         11              
// BR                   motor         20              
// Controller1          controller                    
// BumperA              bumper        A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "functions.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

  while(!BumperA.pressing()){
    FL.spin(fwd);
    FR.spin(fwd);
    BL.spin(fwd);
    BR.spin(fwd);
  }
  FL.stop();
  FR.stop();
  BL.stop();
  BR.stop();                                                                                                                                                                   
}

double moveSpeed = 1; //Global multiplier for drive motor speed; set from 0-1 to adjust max speed

int arcadeTransmission(){
  Brain.Screen.print("Transmission mode: arcade");
  Brain.Screen.newLine();
  while(true){
    FL.spin(directionType::fwd, (Controller1.Axis2.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    FR.spin(directionType::fwd, (Controller1.Axis2.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BL.spin(directionType::fwd, (Controller1.Axis2.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BR.spin(directionType::fwd, (Controller1.Axis2.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int tankTransmission(){
  Brain.Screen.print("Transmission mode: tank");
  Brain.Screen.newLine();
  while(true){
    FL.spin(directionType::fwd, (Controller1.Axis2.position()) * moveSpeed, percentUnits::pct);
    FR.spin(directionType::fwd, (Controller1.Axis3.position()) * moveSpeed, percentUnits::pct);
    BL.spin(directionType::fwd, (Controller1.Axis2.position()) * moveSpeed, percentUnits::pct);
    BR.spin(directionType::fwd, (Controller1.Axis3.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int lift(){

  return 0;
}

//21417A :sob:
void usercontrol(void) {
  Brain.Screen.print("HAL 9000 v0.0.1 // (c) James Haywood 2021");
  Brain.Screen.newLine();
  task manualTransmissionTask = task(tankTransmission); //can be set between "tank" or "arcade"
  //TODO: Manipulator control tasks (need to know some specs first D:<)
  //TODO: Focused EMP firing sequence ;)
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
