#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManagement.hpp"

#define EMERGENCY_STOP abort();

#define DELAY(x) pros::delay(x);

#define TURN(x)

#define CONVEYOR_OFF
#define CONVEYOR_ON
#define CONVEYOR_REVERSE

#define LIFT_FLOOR
#define LIFT_HOVER
#define LIFT_CEIL

#define CLAMP_OPEN
#define CLAMP_CLOSE
#define CLAMP_TOGGLE

#define CLIP_OPEN
#define CLIP_CLOSE
#define CLIP_TOGGLE

#define TRAVERSE_PATH(x)
#define PATHFIND_LINEAR(x)
#define PATHFIND_DISTANCE(x)
#define PATHFIND_ARC(x)