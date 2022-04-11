#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

/**
 * @brief Constrains a given angle to [0, 360) degrees.
 * 
 * @param angle The angle to constrain.
 * @return A constrained copy of the given angle.
 */
QAngle constrainAngle(QAngle angle);

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
void turnRelative(QAngle target_angle);

/**
 * @brief Turn the specified angular distance, regardless of the robot's actual heading.
 * @details For example, if the robot is facing 180 degrees and turnAbsolute(180_deg) is called,
 * the robot will turn 180 degrees to its right.
 * @param total_angle The angular distance.
 */
void turnAbsolute(QAngle total_angle);

/**
 * @brief Turn to face a given coordinate in the robot's odometry frame.
 * @param point The target point.
 */
void turnToPoint(Point point);

/**
 * @brief Turn to face and drive to a given coordinate in the robot's odometry frame.
 * @param point The target point.
 */
void driveToPoint(Point point);