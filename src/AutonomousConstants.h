#pragma once

using namespace okapi::literals;

pros::Imu inertialSensor (21);
double GEAR_RATIO = 84.0/36.0;

auto driveTrain = okapi::ChassisControllerBuilder()
  .withMotors({9, -19}, {-15, 13})
  .withGains
  (
    {0.001, 0, 0.0001}, // Distance controller gains
    {0.001, 0, 0.0001}, // Turn controller gains
    {1, 0, 0.0001}  // Angle controller gains
  )
  // Blue gearset, 4 in wheel diam, 9.5 in wheel track
  .withDimensions(okapi::AbstractMotor::gearset::blue, {{4_in, 9.5_in}, okapi::imev5BlueTPR * GEAR_RATIO})
  .build();

auto pathFinder = okapi::AsyncMotionProfileControllerBuilder()
  .withLimits({10, 5, 10})
  .withOutput(driveTrain->getModel(), {{4_in, 9.5_in}, okapi::imev5BlueTPR * GEAR_RATIO}, {okapi::AbstractMotor::gearset::blue, GEAR_RATIO})
  .buildMotionProfileController();

//Initialize PID front lift controller
auto frontLiftRight = okapi::AsyncPosControllerBuilder()
  .withMotor(1)
  .build();

  //Initialize PID front lift controller
auto frontLiftLeft = okapi::AsyncPosControllerBuilder()
    .withMotor(5)
    .build();

  //Initialize clamp controller
  auto frontClamp = okapi::AsyncPosControllerBuilder()
    .withMotor(8)
    .build();

  //Initialize PID rear lift controller
  auto rearLift = okapi::AsyncPosControllerBuilder()
    .withMotor(6)
    .build();

  //Precalculates all Okapi paths used in auton.
  void initPaths()
  {
      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {3_ft, 0_ft, -45_deg}},
        "BackGrabShort"
      );
  }
