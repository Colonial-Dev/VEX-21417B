#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManager.hpp"

#define EMERGENCY_STOP abort();

#define DELAY(x) pros::delay(x);

#define TURN(x)

#define SETROT(x) inertial_sensor.set_rotation((x).convert(degree));

#define CONVEYOR_OFF conveyor_motor.move_velocity(0);
#define CONVEYOR_ON conveyor_motor.move_velocity(-600);
#define CONVEYOR_REVERSE conveyor_motor.move_velocity(600);

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