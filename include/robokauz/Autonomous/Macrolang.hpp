#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManager.hpp"

#define EMERGENCY_STOP abort();

#define DELAY(x) pros::delay((x).convert(millisecond));

#define TURNREL(x) turnRelative(x);
#define TURNRELGOAL(x) turnRelative(x, true);

#define SETROT(x) inertial_sensor.set_rotation((x).convert(degree));
#define HOLD pros::E_MOTOR_BRAKE_HOLD
#define COAST pros::E_MOTOR_BRAKE_COAST
#define BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define SETBRK(x) overwatch.setDriveBrakeMode(x);

#define CONVEYOR_OFF conveyor_motor.move_velocity(0);
#define CONVEYOR_ON conveyor_motor.move_velocity(-600);
#define CONVEYOR_REVERSE conveyor_motor.move_velocity(600);

#define CLAMP_OPEN clamp_piston.set_value(false);
#define CLAMP_CLOSE clamp_piston.set_value(true);

#define CLIP_OPEN back_piston.set_value(false);
#define CLIP_CLOSE back_piston.set_value(true);

#define LOCK_OPEN lock_piston.set_value(true);
#define LOCK_CLOSE lock_piston.set_value(false);

#define TRAVERSE_PATH(x)
#define PATHFIND_LINEAR(x)
#define PATHFIND_DISTANCE(x)