#pragma once

class Vector;
#define PRINT(x) std::cout << x << std::endl
#define SQ(x) std::pow(x, 2)

template <typename T> int sgnum(T val) 
{
    return (T(0) < val) - (val < T(0));
}

std::string precise_string(double value, const int n = 2);

QAngle constrainAngle360(QAngle angle);

QAngle constrainAngle180(QAngle angle);

QAngle rollAngle180(QAngle angle);

QAngle angleBetweenPoints(Vector start, Vector target);

QAngle angleToPoint(Vector start, Vector target, QAngle heading);