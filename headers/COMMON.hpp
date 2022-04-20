#pragma once

#define PRINT(x) std::cout << x << std::endl
#define SQ(x) std::pow(x, 2)

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

std::string precise_string(double value, const int n = 2);

/**
 * @brief Constrains a given angle to [0, 360) degrees.
 * 
 * @param angle The angle to constrain.
 * @return A constrained copy of the given angle.
 */
QAngle constrainAngle360(QAngle angle);

/**
 * @brief Constrains a given angle to [-180, 180) degrees.
 * 
 * @param angle The angle to constrain.
 * @return A constrained copy of the given angle.
 */
QAngle constrainAngle180(QAngle angle);