#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/Vector2D.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

/**
 * @brief Gets the constrained version of the robot's current rotation.
 */
QAngle getRobotHeading();

/**
 * @brief Turn to face the specified angle as measured relative to zero heading.
 * @details For example, if the robot is facing 180 degrees and turnRelative(270_deg) is called,
 * the robot will turn 90 degrees to its right.
 * @param target_angle The target angle.
 */
void turnRelative(QAngle target_angle, bool goal = false);