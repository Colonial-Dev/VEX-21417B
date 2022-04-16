#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Other/BotManager.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManagement.hpp"

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
  SimpleRush, //Go straight forwards, grab the mobile goal, and return.
  DoubleRush, //Go straight forwards, grab the mobile goal, turn and grab the colored goal, and finally return.
  StackRush, //Grab the colored goal as a counterweight, then pick up the middle mobile goal and return.
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

extern const RobotProperties robot_properties;
extern PathManager wayfarer;