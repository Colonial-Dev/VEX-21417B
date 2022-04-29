#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"

std::string precise_string(double value, const int n)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << value;
    return out.str();
}

QAngle constrainAngle360(QAngle angle)
{
    double x = angle.convert(degree);
    x = std::fmod(x, 360);
    if (x < 0) { x += 360; }
    return QAngle (x * degree);
}

QAngle constrainAngle180(QAngle angle)
{
    double x = angle.convert(degree);
    x = std::fmod(x + 180, 360);
    if(x < 0) { x += 360; }
    return (x - 180) * degree;
}

QAngle rollAngle180(QAngle angle)
{
    return (angle.convert(radian) - (2 * M_PI) * std::floor((angle.convert(radian) + M_PI) / (2* M_PI))) * radian;
}

QAngle angleBetweenPoints(Vector start, Vector target)
{
    QAngle atan = std::atan2(target.y_component.convert(meter) - start.y_component.convert(meter), target.x_component.convert(meter) - start.x_component.convert(meter)) * radian;
    return rollAngle180(atan);
}

QAngle angleToPoint(Vector start, Vector target, QAngle heading)
{
    return rollAngle180(angleBetweenPoints(start, target) - heading);
}