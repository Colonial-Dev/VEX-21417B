//Note: COMMON includes a grab-bag of macro definitions and function/class declarations that are useful throughout the program,
//but don't need to be placed in the PRELUDE.
#pragma once

/**
 * @brief PRINT outputs the provided string to the standard output (PROS terminal).
 * @details Example usage: PRINT("Motor temp: " + std::to_string(motor.get_temperature()));
 */
#define PRINT(x) std::cout << x << std::endl

/**
 * @brief Squares the provided number.
 */
#define SQ(x) std::pow(x, 2)

#define MOTOR_MAX_VOLTAGE 12000

//Forward declaration of Vector2D class to resolve circular inclusions.
class Vector2D;

/**
 * @brief Sign number template. For the given numerical value, returns 1 if positive and -1 if negative.
 * 
 * @tparam T Any base numerical type.
 * @param val The numerical value to obtain the sign number of.
 */
template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

/**
 * @brief Convert a double value to a string, rounded to the specified number of decimal places. 
 * @param value The double value to stringify.
 * @param n The number of decimal places to preserve.
 */
std::string precise_string(double value, const int n = 2);

/**
 * @brief Constrain a QAngle to between 0 and 360 degrees (0 inclusive, 360 exclusive.)
 * @param angle The QAngle to constrain.
 */
QAngle constrainAngle360(QAngle angle);

/**
 * @brief Constrain a QAngle to between -180 and 180 degrees.
 * @param angle The QAngle to constrain.
 */
QAngle constrainAngle180(QAngle angle);

/**
 * @brief Identical (I think) to constrainAngle180 - this function was adapted from elsewhere.
 * @param angle The QAngle to constrain.
 */
QAngle rollAngle180(QAngle angle);

/**
 * @brief Finds the absolute angle between two cartesian (Vector2D) points.
 * @param start The origin point.
 * @param target The destination point.
 */
QAngle angleBetweenPoints(Vector2D start, Vector2D target);

/**
 * @brief Finds the angle to a point, given a start/end Vector2D and a heading.
 * @param start The origin point.
 * @param target The destination point.
 * @param heading The heading to use in the calculation.
 */
QAngle angleToPoint(Vector2D start, Vector2D target, QAngle heading);