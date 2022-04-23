#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

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

void initializeOpcontrol();