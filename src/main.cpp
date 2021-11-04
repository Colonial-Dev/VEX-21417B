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
// MainLiftLeft         motor         13              
// LeftLiftMotor        motor         11              
// RightLiftMotor       motor         12              
// MainLiftRight        motor         15              
// Inertial             inertial      5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "functions.h"
#include "cmath"

using namespace vex;

// A global instance of competition
competition Competition;
int liftSpeed = 100;

// define your global instances of motors and other devices here

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

/*Use inbuilt motor encoders to accurately drive forwards (and hopefully backwards) for provided distance
Need to figure out the ratio of say 100 units to in/ft/whatever*/ 

void drivePID(double clicks){
  clicks = -clicks;
  FrontLeft.resetPosition();
  FrontRight.resetPosition();
  double averagePosition = (FrontLeft.position(deg) + FrontRight.position(deg)) / 2;
  double threshold = 10.0;
	double error = clicks - averagePosition;

	double derivative;
	double prevError;

	double kp = 0.1;//0.98;
	double kd = 0.1;//5.5;

  double rightPower = 0;
  double leftPower = 0;

  while (fabs(error) > threshold) {
    averagePosition = (FrontLeft.position(deg) + FrontRight.position(deg)) / 2;
	  error = clicks - averagePosition;
    Brain.Screen.print(error);
    Brain.Screen.newLine();
    derivative = error - prevError;

    rightPower = leftPower + (error * kp) + (derivative * kd);

    FrontLeft.spin(directionType::fwd, rightPower, percentUnits::pct);
    FrontRight.spin(directionType::fwd, rightPower, percentUnits::pct);
    BackLeft.spin(directionType::fwd, rightPower, percentUnits::pct);
    BackRight.spin(directionType::fwd, rightPower, percentUnits::pct);

    prevError = error;
    wait(20, msec);
  }
  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
}

/*
Use the bot's inertial sensor to orient to a heading.
The heading value provided will be treated as relative to 0 (e.g. the direction the bot was facing when turned on)
So example: if you're facing 0 and call this method for 90, the bot will turn right 90 degrees
Then if you call 0, it'll turn left 90 degrees
*/

void turnPID(double angle){

  while(Inertial.isCalibrating())
  {
    wait(10,msec); 
  }

	double threshold;
	if(angle <= 0.0)
  {
		threshold = 1.5;
	}
	else
  {
		threshold = 0.7;
	}

	double error = angle - Inertial.rotation();
	double integral;
	double derivative;
	double prevError;
	double kp = 0.98;
	double ki = 0.001;
	double kd = 5.5;

	while(fabs(error) > threshold)
  {
    error = angle - Inertial.rotation();
		integral  = integral + error;

		if(error == 0 || fabs(error) >= angle)
    {
			integral = 0;
		}

		derivative = error - prevError;
		prevError = error;
		double p = error * kp;
		double i = integral * ki;
		double d = derivative * kd;
		double vel = p + i + d;

    FrontLeft.spin(directionType::fwd, -vel, percentUnits::pct);
    FrontRight.spin(directionType::fwd, vel, percentUnits::pct);
    BackLeft.spin(directionType::fwd, -vel, percentUnits::pct);
    BackRight.spin(directionType::fwd, vel, percentUnits::pct);

		wait(15,msec);
	}
  FrontLeft.stop();
  FrontRight.stop();
  BackLeft.stop();
  BackRight.stop();
}

void autonomous(void) {
  turnPID(180);
  //wait(5,sec);
  turnPID(0);
  drivePID(1500);
  drivePID(-1500);                                                                                                                        
}

double moveSpeed = 1; //Global multiplier for drive motor speed; set from 0-1 to adjust max speed

int tankTransmission(){ //Controls: Axes 3 and 2 control left and right fwd/bckwd. Axis 4 controls lateral drive.
  Brain.Screen.print("Transmission mode: TANK/LATERAL");
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

int mainLiftControl(){
  //R1 up, R2 down
  MainLiftLeft.setVelocity(liftSpeed, percentUnits::pct); //Adjust from 0-100 to determine speed of LiftDrivers
  MainLiftRight.setVelocity(liftSpeed, percentUnits::pct);
  while(true){
    if(Controller1.ButtonR1.pressing()){
      MainLiftLeft.spin(directionType::fwd);
      MainLiftRight.spin(directionType::fwd);
    }
    else if(Controller1.ButtonR2.pressing()){
      MainLiftLeft.spin(directionType::rev);
      MainLiftRight.spin(directionType::rev);
    }
    else{
      MainLiftLeft.stop(brakeType::hold);
      MainLiftRight.stop(brakeType::hold);
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

//21417A 
void usercontrol(void) {
  Brain.Screen.print("HAL 9000 // (c) 21417A 2021");
  Brain.Screen.newLine();
  task manualTransmissionTask = task(tankTransmission); //can be set between "tank" or "arcade"
  task mainLiftTask = task(mainLiftControl); //control lift
  task leftLiftTask = task(leftLiftControl);
  task rightLiftTask = task(rightLiftControl);
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
