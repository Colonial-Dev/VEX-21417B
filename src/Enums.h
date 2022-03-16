#pragma once

enum RobotStatus
{
  Driver,
  Auton,
  Disabled
};

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

enum GUIPage
{
  Home,
  Logs,
  Odom,
  Debug
};

enum AutonSelectionStage
{
  Side,
  Strategy,
  Complete
};
