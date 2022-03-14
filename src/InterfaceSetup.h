#pragma once
#include "DisplaySuite.h"

//Global UI constants
bool readyToLaunch = false;
bool autonJumpFlag = false;
bool driverJumpFlag = false;
bool hotkeyMode = false;
int targetAutonSide = Right;
std::string targetAutonSideLabel = "R";
int targetAutonStrategy = DoubleRush;
std::string targetAutonStrategyLabel = "DR";
float throttleMultiplier = 1.0f;
