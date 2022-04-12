#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#define WAIT_FOR_AUTH pros::Task::notify_take(true, TIMEOUT_MAX); pros::Task::current().notify_ext(1, NOTIFY_ACTION_OWRITE, NULL);

//Axis 3 is translation, axis 1 is rotation
void splitTransmission();

//L1 up, L2 down
void mainLiftControl();

//B to close, Y to open
void frontClampControl();

//Down to close, right to open
void rearClampControl();

//X to close, A to open
void topClampControl();

//R1 forward, R2 reverse
void conveyorControl();