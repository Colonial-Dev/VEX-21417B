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
}

bool isWall(distance dist){
    if(dist.isObjectDetected() && dist.objectVelocity() == 0 && dist.objectSize() == sizeType::large){ //if the object is stationary and large, its probably a wall
      return true;
    }
    else{
      return false;
    }
}

/*int getWallDistance(distance dist){
    int ret = 0;

    for(int i  = 0; i < 5; i++){
      if(isWall(dist)){ //if the object is stationary and large, its probably a wall
        ret += North.objectDistance(distanceUnits::mm);
      }
      wait(20, msec); 
    }

    ret = ret / 5;
  
    if(ret == 0){
      ret = -1;
    }

    return ret;
}*/

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

/*void orientToHeading(int h){
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
}*/

/*
Return the bot's UFC using distance sensors. No input arguments; returns an array with the millimeter x and y coords in it.
THIS WILL LOCK UP THE BOT FOR A FEW SECONDS! Current implementation requires the bot to orient to zero heading to get a position lock.
*/

/*int * getUFC(){ 
  static int coords[2];
  int northDistance;
  int southDistance;
  int eastDistance;
  int westDistance;

  orientToHeading(0);

  northDistance = getWallDistance(North);
  southDistance = getWallDistance(South);
  eastDistance = getWallDistance(East);
  westDistance = getWallDistance(West);

  //Possible combos are NE, SE, SW, NW
  //NE is an inverted measure
  //SE is inverted on the x axis
  //SW is a direct measure
  //NW is inverted on the y axis
  //12ft = 3657.6mm

  static int NE[2];
  static int SE[2];
  static int SW[2];
  static int NW[2];

  int validPairs = 0;

  if(isValidCombo(northDistance, eastDistance)){
    NE[0] = (3657.6 - northDistance);
    NE[1] = (3657.6 - eastDistance);
    validPairs++;
  }
  else{
    NE[0] = 0;
    NE[1] = 0;
  }

  if(isValidCombo(southDistance, eastDistance)){
    SE[0] = southDistance;
    SE[1] = (3657.6 - eastDistance);
    validPairs++;
  }
  else{
    SE[0] = 0;
    SE[1] = 0;
  }

  if(isValidCombo(southDistance, westDistance)){
    SW[0] = southDistance;
    SW[1] = westDistance;
    validPairs++;
  }
  else{
    SW[0] = 0;
    SW[1] = 0;
  }

  if(isValidCombo(northDistance, westDistance)){
    NW[0] = (3657.6 - northDistance);
    NW[1] = eastDistance;
    validPairs++;
  }
  else{
    NW[0] = 0;
    NW[1] = 0;
  }

  int xCoord = (NE[1] + SE[1] + SW[1] + NW[1]) / validPairs;
  int yCoord = (NE[0] + SE[0] + SW[0] + NW[0]) / validPairs;

  coords[0] = xCoord;
  coords[1] = yCoord;
  
  return coords;
}*/

/*
Given the bot's current UFC and a destination UFC, move to the destination coordinates.
Operates by orienting the bot to zero, then translating the required Y distance followed by the X distance, or vice versa. Does not account for obstacles; pathing around known
objects will have to be handled with multiple goTo commands.

NOTE: You can feed the destination UFC in to a subsequent goTo command, but drift from this will compound FAST. Best to run getUFC between moves if at all possible.
*/

void goToUFC(int currX, int currY, int destX, int destY, bool translationMode){
  
}

/*
Given a distance in mm and a "movement mode" (e.g. fwd, back, left, right), this will move the bot that distance that way.
This function will be calibrated specifically to our bot, as the required revs will be calculated based off the distance travelled in one rev.  
*/

void moveDistance(int dist, int mode){ //1 fwd, 2 back, 3 left, 4 right

}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................   
  //                  
  /*
  DISTANCE SENSORS - range of 6.5 ft. Field is 12ft wide. Can distinguish object sizes, supposedly well enough to detect walls. Also can get closing velocity;
  if its nonzero w/ the motors off we can know its an opponent bot. Therefore, from all pts on the field, it should be theoretically possible to get the distance
  to at least two perpendicular walls - and from that, calculate the bot's field position.

  Algorithm:

  4x distance, 1x inertial, a lot of cocaine

  1. Orient bot to 0 heading as determined by inertial sensor, which should place all beams perpendicular to the walls.

  2. Lock brakes. Any movement will interefere with obtaining the bot's Universal Field Coordinates.

  3. Poll each distance sensor for readings. Discard any non-large readings, or any readings with a nonzero closing velocity (which would indicate an enemy bot) 
  3A. Re-poll any sensors with discarded readings several times to try and get as much valid data as possible
  3B. If distance readout varies per-poll, then average them

  4. If at least one valid wall distance reading has been found on each axis (x and y) then simple math can be done to find the robot's Universal Field Coordinates as mm x/y values.
  4A. If 3-4 sensors can get a valid reading, then each possible set should run the UFC calculations and all outputs should be averaged together to help account for the
  5% accuracy variance of the sensor

  5. Write a function that encapsulates all coordinate-finding logic in one call (returns x,y values as int array)
  
  6. Write a function that can take current coords and destination coords, and automagically move the bot to those coordinates

  7. Other functions - rotate to heading? Move x distance in x manner (sideways, etc)?
  */                                                                                                                             
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

int tankTransmission(){ //Controls: Axes 3 and 2 control left and right fwd/bckwd. Axis 1 controls lateral drive.
  Brain.Screen.print("Transmission mode: TANK");
  Brain.Screen.newLine();
  Brain.Screen.print("Mecanum lateral drive: ENABLED"); //should work
  Brain.Screen.newLine();
  while(true){
    FrontLeft.spin(directionType::rev, (Controller1.Axis3.position() + Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    BackLeft.spin(directionType::rev, (Controller1.Axis3.position() - Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    FrontRight.spin(directionType::rev, (Controller1.Axis2.position() - Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    BackRight.spin(directionType::rev, (Controller1.Axis2.position() + Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int fwdLiftControl(){
  //R1 up, R2 down
  FwdLiftMotor.setVelocity(30, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
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
  LeftLiftMotor.setVelocity(30, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
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
  RightLiftMotor.setVelocity(30, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
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
  RearLiftMotor.setVelocity(30, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonL1.pressing()){
      RearLiftMotor.spin(directionType::fwd);
    }
    else if(Controller1.ButtonL2.pressing()){
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
