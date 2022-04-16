#include "robokauz/PROS.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"

Vector::Vector()
{
    x_component = 0_in;
    y_component = 0_in;
}

Vector::Vector(QLength x_comp, QLength y_comp)
{
    x_component = x_comp;
    y_component = y_comp;
}

Vector::Vector(QLength magnitude, QAngle direction)
{
    magnitude = magnitude.abs();
    x_component = (magnitude.convert(meter) * std::cos(direction.convert(radian))) * meter;
    y_component = (magnitude.convert(meter) * std::sin(direction.convert(radian))) * meter;
}

Vector::Vector(RawPoint point)
{
    x_component = point.x_pos;
    y_component = point.y_pos;
}

Vector::Vector(PathPoint point)
{
    x_component = point.x_pos;
    y_component = point.y_pos;   
}

Vector::Vector(OdomState position)
{
    x_component = position.x;
    y_component = position.y;
}

QLength Vector::magnitude()
{
    double x_pow = std::pow((x_component.convert(meter)), 2);
    double y_pow = std::pow((y_component.convert(meter)), 2);
    double rooted = std::sqrt(x_pow + y_pow);
    QLength qlen = (rooted * meter);
    return qlen.abs();
}

QAngle Vector::direction()
{
    double theta = std::atan2(y_component.convert(meter), x_component.convert(meter)) * 180 / pi;
    double corrected = std::fmod((theta + 360), 360); 
    QAngle qang (corrected * degree);
    return qang;
}

QAngle Vector::angleFrom(OdomState position)
{
    Vector difference = Vector(position) - *this;
    QAngle angle = (std::atan2(difference.x_component.convert(meter), difference.y_component.convert(meter)) * radian) - position.theta;
    return angle - 360.0_deg * std::floor((angle.convert(degree) + 180.0) / 360.0);
}

QLength Vector::distance(Vector target)
{
    double x_components = std::pow(target.x_component.convert(meter) - x_component.convert(meter), 2);
    double y_components = std::pow(target.y_component.convert(meter) - y_component.convert(meter), 2);
    return ((std::sqrt(x_components + y_components)) * meter);
}

Vector Vector::normalize()
{
    Vector normalizedVector;
    QLength mag = magnitude();
    if(mag.convert(meter) != 0)
    { 
        normalizedVector.x_component =  QLength ((x_component.convert(meter) / mag.convert(meter)) * meter);
        normalizedVector.y_component =  QLength ((y_component.convert(meter) / mag.convert(meter)) * meter);
    }
    return normalizedVector;
}

double Vector::dot(Vector multiplicand)
{
    return (x_component.convert(meter) * multiplicand.x_component.convert(meter)) + (y_component.convert(meter) * multiplicand.y_component.convert(meter));
}


QLength interpointDistance(Vector point_a, Vector point_b)
{
    double x_components = std::pow(point_b.x_component.convert(meter) - point_a.x_component.convert(meter), 2);
    double y_components = std::pow(point_b.y_component.convert(meter) - point_a.y_component.convert(meter), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * meter);
    return qlen;
}