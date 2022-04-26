#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Other/BotManager.hpp"
#include "robokauz/Other/TaskGates.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

//Robot state enums
enum ConveyorStatus
{
  Idle,
  Forward,
  Reverse
};

enum AutonSide
{
  Left,
  Right,
  Skills,
  None
};

enum AutonStrat
{
  FullWinPoint, //Full win point auton (left side only?)
  HalfWinPoint, //Half win point (side specific)
  GoalRush, //Grab a small + large neutral
  StackRush //Grab and stack a small + large neutral
};

extern pros::Controller master;

extern pros::Motor right_back;
extern pros::Motor right_middle;
extern pros::Motor right_front;
extern pros::Motor left_back;
extern pros::Motor left_middle;
extern pros::Motor left_front; 

extern pros::Motor arm_motor;
extern pros::Motor conveyor_motor;
extern pros::ADIDigitalOut back_piston;
extern pros::ADIDigitalOut clamp_piston;
extern pros::ADIDigitalOut top_piston;
extern pros::ADIDigitalOut lock_piston;

extern pros::IMU inertial_sensor;
extern pros::ADIEncoder left_encoder;
extern pros::ADIEncoder right_encoder;
extern pros::ADIEncoder middle_encoder;
extern pros::ADIAnalogIn potentiometer;

extern const double GEAR_RATIO;

extern std::shared_ptr<okapi::OdomChassisController> drive_train;

extern EncoderGroup encoders;
extern IMUOdometer imu_odometer;

extern BotManager overwatch;
extern TaskGate driver_control_gate;
extern LiftController arm_controller;

extern const RobotProperties robot_properties;
extern PathManager wayfarer;