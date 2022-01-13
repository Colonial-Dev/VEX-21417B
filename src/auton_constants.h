#pragma once

using namespace okapi::literals;

//Global auton constants
pros::Imu inertialSensor (21);

auto driveTrain = okapi::ChassisControllerBuilder()
  .withMotors({9, -19}, {-15, 13})
  // Blue gearset, 4 in wheel diam, 9.5 in wheel track
  .withDimensions(okapi::AbstractMotor::gearset::blue, {{4_in, 24.13_cm}, okapi::imev5BlueTPR})
  .build();

auto pathFinder = okapi::AsyncMotionProfileControllerBuilder()
  .withOutput(driveTrain->getModel(), {{4_in, 24.13_cm}, okapi::imev5BlueTPR}, {okapi::AbstractMotor::gearset::blue})
  .withLimits({.2, .41, 1})
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

  void initPaths()
  {
      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {8.7_ft, 0_ft, 0_deg}},
        "RushStraight"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {7_ft, 0_ft, 0_deg}},
        "PeekOut"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {6_ft, 0_ft, 0_deg}},
        "PeekOutShort"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {8.8_ft, 0_ft, 0_deg}},
        "RushMiddle"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {6.3_ft, 0_ft, 0_deg}},
        "RushMiddleShort"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {4.7_ft, 0_ft, 0_deg}},
        "BackGrab"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {10_ft, 0_ft, 0_deg}},
        "BackGrabLong"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {3_ft, 0_ft, 0_deg}},
        "BackGrabShort"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {1.5_ft, 0_ft, 0_deg}},
        "BackGrabTiny"
      );

      pathFinder->generatePath({
        {0_ft, 0_ft, 0_deg},
        {4.9_ft, 0_ft, 0_deg}},
        "Park"
      );
  }
