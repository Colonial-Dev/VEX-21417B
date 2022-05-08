//Note: ROBOT stores extern declarations of all variables that need to be shared throughout the program, such as motors or controller instances.
//It also holds a few enums that the entire program needs to know about.
#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Other/BotManager.hpp"
#include "robokauz/Other/TaskGate.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

enum AutonSide
{
  Left,
  Right,
  Skills,
  None
};

enum AutonStrat
{
  FullWinPoint, //Full win point auton (Not necessarily left side only)
  HalfWinPoint, //Half win point (side specific)
  SingleGoalRush,
  MiddleGoalRush 
};

//The primary V5 controller.
extern pros::Controller master;

//All drive motors.
extern pros::Motor right_back;
extern pros::Motor right_middle;
extern pros::Motor right_front;
extern pros::Motor left_back;
extern pros::Motor left_middle;
extern pros::Motor left_front; 

//All manipulator motors and pistons.
extern pros::Motor arm_motor;
extern pros::Motor conveyor_motor;
extern pros::ADIDigitalOut back_piston;
extern pros::ADIDigitalOut clamp_piston;
extern pros::ADIDigitalOut top_piston;
extern pros::ADIDigitalOut lock_piston;

//All sensors.
extern pros::IMU inertial_sensor;
extern pros::ADIEncoder left_encoder;
extern pros::ADIEncoder right_encoder;
extern pros::ADIEncoder middle_encoder;
extern pros::ADIAnalogIn potentiometer;

//Constant defining the gear ratio between the V5 smart motors and drive wheels.
extern const double GEAR_RATIO;

//All motor controllers.
extern LiftController arm_controller;
extern ConveyorController conveyor_controller;
extern std::shared_ptr<DriveController> drive_controller;
extern std::shared_ptr<okapi::ChassisController> okapi_controller;

//IMU odometer encoders and object.
extern EncoderGroup encoders;
extern IMUOdometer imu_odometer;

//Overwatch and task management.
extern BotManager overwatch;
extern TaskGate driver_control_gate;

//Pure pursuit objects.
extern const RobotProperties robot_properties;
extern PathManager wayfarer;