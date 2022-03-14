#pragma once

#define EMERGENCY_STOP abort();

#define DELAY(x) \
pros::delay(x);

#define TURN(x) inertialTurn(x);