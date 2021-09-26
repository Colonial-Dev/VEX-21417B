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
// FrontLeft            motor         1               
// FrontRight           motor         16              
// BackLeft             motor         9               
// BackRight            motor         19              
// Controller1          controller                    
// FwdLiftMotor         motor         13              
// LeftLiftMotor        motor         11              
// RightLiftMotor       motor         12              
// RearLiftMotor        motor         15              
// Inertial             inertial      5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "functions.h"

using namespace vex;

// A global instance of competition
competition Competition;
int liftSpeed = 100;

// define your global instances of motors and other devices here

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

bool isWall(distance dist){
    if(dist.isObjectDetected() && dist.objectVelocity() == 0 && dist.objectSize() == sizeType::large){ //if the object is stationary and large, its probably a wall
      return true;
    }
    else{
      return false;
    }
}

bool isValidCombo(int x, int y){
  if(x != -1 && y != -1){
    return true;
  }
  else{
    return false;
  }
}

/*
Use the bot's inertial sensor to orient to a heading.
*/

void orientToHeading(int h){
  int currHeading = Inertial.heading();
  if(currHeading > h){
    while(Inertial.heading() <= (h - 0.1) || Inertial.heading() >= (h + 0.1)){
      //turn left until heading is reached, then return
    }
  }
  else if(currHeading < h){
    while(Inertial.heading() <= (h - 0.1) || Inertial.heading() >= (h + 0.1)){
      //turn right until heading is reached, then return
    }
  }
}

double Kp = 0;
//double Ki = 0;
double Kd = 0;

double error = 0;
double lastError = 0;
//double integral = 0;
double derivative = 0;

void drivePID(double dist){
  while()
  return;
}


void autonomous(void) {
  orientToHeading(180);                                                                                                                        
}

double moveSpeed = 1; //Global multiplier for drive motor speed; set from 0-1 to adjust max speed

int arcadeTransmission(){ //Controls: Axis 2 is fwd/back. Axis 1 is turn. Axis 4 is lateral drive.
  Brain.Screen.print("Transmission mode: ARCADE");
  Brain.Screen.newLine();
  Brain.Screen.print("Mecanum lateral drive: ENABLED"); //should work
  Brain.Screen.newLine(); 
  while(true){
    FrontLeft.spin(directionType::fwd, (Controller1.Axis2.position() + Controller1.Axis1.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BackLeft.spin(directionType::rev, (Controller1.Axis2.position() + Controller1.Axis1.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    FrontRight.spin(directionType::fwd, (Controller1.Axis2.position() - Controller1.Axis1.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BackRight.spin(directionType::rev, (Controller1.Axis2.position() - Controller1.Axis1.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int tankTransmission(){ //Controls: Axes 3 and 2 control left and right fwd/bckwd. Axis 4 controls lateral drive.
  Brain.Screen.print("Transmission mode: TANK");
  Brain.Screen.newLine();
  Brain.Screen.print("Mecanum lateral drive: ENABLED"); //should work
  Brain.Screen.newLine();
  Brain.Screen.print("Current lift motor speed: " + liftSpeed);
  Brain.Screen.newLine();
  while(true){
    FrontLeft.spin(directionType::rev, (Controller1.Axis3.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BackLeft.spin(directionType::rev, (Controller1.Axis3.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    FrontRight.spin(directionType::rev, (Controller1.Axis2.position() - Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    BackRight.spin(directionType::rev, (Controller1.Axis2.position() + Controller1.Axis4.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int fwdLiftControl(){
  //R1 up, R2 down
  FwdLiftMotor.setVelocity(liftSpeed, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonR1.pressing()){
      FwdLiftMotor.spin(directionType::fwd);
    }
    else if(Controller1.ButtonR2.pressing()){
      FwdLiftMotor.spin(directionType::rev);
    }
    else{
      FwdLiftMotor.stop(brakeType::hold);
    }
    wait(20, msec);
  }
  return 0;
}

int leftLiftControl(){
  LeftLiftMotor.setVelocity(liftSpeed, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonUp.pressing()){
      LeftLiftMotor.spin(directionType::fwd);
    }
    else if(Controller1.ButtonDown.pressing()){
      LeftLiftMotor.spin(directionType::rev);
    }
    else{
      LeftLiftMotor.stop(brakeType::hold);
    }
    wait(20, msec);
  }
  return 0;
}

int rightLiftControl(){
  RightLiftMotor.setVelocity(liftSpeed, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonX.pressing()){
      RightLiftMotor.spin(directionType::fwd);
    }
    else if(Controller1.ButtonB.pressing()){
      RightLiftMotor.spin(directionType::rev);
    }
    else{
      RightLiftMotor.stop(brakeType::hold);
    }
    wait(20, msec);
  }
  return 0;
}

int rearLiftControl(){
  RearLiftMotor.setVelocity(liftSpeed, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonL2.pressing()){
      RearLiftMotor.spin(directionType::fwd);
    }
    else if(Controller1.ButtonL1.pressing()){
      RearLiftMotor.spin(directionType::rev);
    }
    else{
      RearLiftMotor.stop(brakeType::hold);
    }
    wait(20, msec);
  }
  return 0;
}

//21417A 
void usercontrol(void) {
  Brain.Screen.print("HAL 9000 v0.1 // (c) James Haywood 2021");
  Brain.Screen.newLine();
  task manualTransmissionTask = task(tankTransmission); //can be set between "tank" or "arcade"
  task fwdLiftTask = task(fwdLiftControl); //control lift
  task leftLiftTask = task(leftLiftControl);
  task rightLiftTask = task(rightLiftControl);
  task rearLiftTask = task(rearLiftControl);
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
