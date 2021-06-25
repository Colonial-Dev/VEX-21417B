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
// FrontLeft            motor         12              
// FrontRight           motor         20              
// BackLeft             motor         1               
// BackRight            motor         10              
// Controller1          controller                    
// LiftDrivers          motor_group   2, 9            
// Inertial             inertial      15              
// North                distance      16              
// South                distance      17              
// East                 distance      18              
// West                 distance      19              
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

/*
Return the bot's UFC using distance sensors. No input arguments; returns an array with the millimeter x and y coords in it.
THIS WILL LOCK UP THE BOT FOR SEVERAL SECONDS! Current implementation requires the bot to orient to zero heading to get a position lock.
*/

void orientToHeading(int h){
  int currHeading = Inertial.heading();
  if(currHeading > h){
    while(Inertial.heading() <= (h - 0.5) || Inertial.heading() >= (h + 0.5)){
      //turn left until heading is reached, then return
    }
  }
  else if(currHeading < h){
    while(Inertial.heading() <= (h - 0.5) || Inertial.heading() >= (h + 0.5)){
      //turn right until heading is reached, then return
    }
  }
}

int * getUFC(){ 
  static int coords[2];
  int northDistance;
  int southDistance;
  int eastDistance;
  int westDistance;

  orientToHeading(0);

  if(North.isObjectDetected() && North.objectVelocity() == 0 && North.objectSize() == sizeType::large){ //if the object is stationary and large, its probably a wall
    northDistance = North.objectDistance(distanceUnits::mm);
  }
  
  return coords;
}

/*
Given the bot's current UFC and a destination UFC, move to the destination coordinates.
Operates by orienting the bot to zero, then translating the required Y distance followed by the X distance. Does not account for obstacles; pathing around known
objects will have to be handled with multiple goTo commands.

NOTE: You can feed the destination UFC in to a subsequent goTo command, but drift from this will compound FAST. Best to run getUFC between moves if at all possible.
*/

void goToUFC(int currX, int currY, int destX, int destY){
  
}

/*
Use the bot's inertial sensor to orient to a fixed heading.
*/

/*
Given a distance in mm and a "movement manner" (e.g. fwd, back, left, right), this will move the bot that distance that way.
This function will be calibrated specifically to our bot, as the required revs will be calculated based off the distance travelled in one rev.  
*/

void moveDistance(int dist, std::string manner){

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
    FrontLeft.spin(directionType::fwd, (Controller1.Axis2.position() + Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    BackLeft.spin(directionType::rev, (Controller1.Axis2.position() - Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    FrontRight.spin(directionType::fwd, (Controller1.Axis3.position() - Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    BackRight.spin(directionType::rev, (Controller1.Axis3.position() + Controller1.Axis1.position()) * moveSpeed, percentUnits::pct);
    wait(20, msec);
  }
  return 0;
}

int liftControl(){
  //R1 up, R2 down
  LiftDrivers.setVelocity(30, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  while(true){
    if(Controller1.ButtonR1.pressing()){
      LiftDrivers.spin(directionType::fwd);
    }
    else if(Controller1.ButtonR2.pressing()){
      LiftDrivers.spin(directionType::rev);
    }
    else{
      LiftDrivers.stop(brakeType::hold);
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
  task liftControlTask = task(liftControl); //control lift
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
