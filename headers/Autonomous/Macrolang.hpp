#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManager.hpp"

#define EMERGENCY_STOP abort();

#define DELAY(x) pros::delay(x);

#define TURN(x)

#define CONVEYOR_OFF
#define CONVEYOR_ON
#define CONVEYOR_REVERSE

#define LIFT_FLOOR
#define LIFT_HOVER
#define LIFT_CEIL

#define CLAMP_OPEN clamp_piston.set_value(false);
#define CLAMP_CLOSE clamp_piston.set_value(true);
#define CLAMP_TOGGLE clamp_piston.set_value(!clamp_piston.get_value());

#define CLIP_OPEN back_piston.set_value(false);
#define CLIP_CLOSE back_piston.set_value(true);
#define CLIP_TOGGLE back_piston.set_value(!back_piston.get_value());

#define TRAVERSE_PATH(x)
#define PATHFIND_LINEAR(x)
#define PATHFIND_DISTANCE(x)
#define PATHFIND_ARC(x)