#ifndef AUTON_CONSTANTS_H
#define AUTON_CONSTANTS_H

using namespace okapi::literals;

//Global auton constants
std::shared_ptr<okapi::ChassisController> driveTrain =
  okapi::ChassisControllerBuilder()
    .withMotors({9, -19}, {-15, 13})
    .withGains
    (
      {0.001, 0, 0.0001}, // Distance controller gains
      {0.001, 0, 0.0001}, // Turn controller gains
      {1, 0, 0.0001}  // Angle controller gains
    )
    // Blue gearset, 4 in wheel diam, 9.5 in wheel track
    .withDimensions(okapi::AbstractMotor::gearset::blue, {{4_in, 9.5_in}, okapi::imev5BlueTPR})
    .build();

std::shared_ptr<okapi::AsyncMotionProfileController> pathFinder =
okapi::AsyncMotionProfileControllerBuilder()
  .withLimits({
    50.0, // Maximum linear velocity of the Chassis in m/s
    5.0, // Maximum linear acceleration of the Chassis in m/s/s
    1000.0 // Maximum linear jerk of the Chassis in m/s/s/s
  })
  .withOutput(driveTrain)
  .buildMotionProfileController();

  //Initialize PID front lift controller
  std::shared_ptr<okapi::AsyncPositionController<double, double>> frontLiftRight =
  okapi::AsyncPosControllerBuilder()
    .withMotor(1)
    .build();

  //Initialize PID front lift controller
  std::shared_ptr<okapi::AsyncPositionController<double, double>> frontLiftLeft =
  okapi::AsyncPosControllerBuilder()
    .withMotor(5)
    .build();

  //Initialize clamp controller
  std::shared_ptr<okapi::AsyncPositionController<double, double>> frontClamp =
  okapi::AsyncPosControllerBuilder()
    .withMotor(8)
    .build();

  //Initialize PID rear lift controller
  std::shared_ptr<okapi::AsyncPositionController<double, double>> rearLift =
  okapi::AsyncPosControllerBuilder()
    .withMotor(6)
    .build();

#endif
