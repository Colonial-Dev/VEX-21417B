#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

QAngle constrainAngle(QAngle angle);

QAngle getRobotHeading();

void turnRelative(QAngle target_angle);

void turnAbsolute(QAngle total_angle);

void turnToPoint(Point point);

void driveToPoint(Point point);