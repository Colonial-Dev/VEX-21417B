#pragma once

enum AutonSide
{
  Null,
  Left,
  Right,
  Skills
};

enum AutonStrat
{
  SimpleRush, //Go straight forwards, grab the mobile goal, and return.
  SpinRush, //Go straight forwards, grab the mobile goal, turn and grab the colored goal, and finally return.
  MiddleRush, //Grab the colored goal as a counterweight, then pick up the middle mobile goal and return.
};
