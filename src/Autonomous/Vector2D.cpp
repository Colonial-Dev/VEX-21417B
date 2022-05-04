#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/Vector2D.hpp"

Vector2D::Vector2D()
{
    x_component = 0_in;
    y_component = 0_in;
}

Vector2D::Vector2D(QLength x_comp, QLength y_comp)
{
    x_component = x_comp;
    y_component = y_comp;
}

Vector2D::Vector2D(QLength magnitude, QAngle direction)
{
    magnitude = magnitude.abs();
    direction = constrainAngle360(direction);
    
    x_component = (magnitude.convert(meter) * std::cos(direction.convert(radian))) * meter;
    y_component = (magnitude.convert(meter) * std::sin(direction.convert(radian))) * meter;
}

Vector2D::Vector2D(SplinePoint point)
{
    x_component = point.x_pos;
    y_component = point.y_pos;
}

Vector2D::Vector2D(PathPoint point)
{
    x_component = point.x_pos;
    y_component = point.y_pos;   
}

Vector2D::Vector2D(OdomState position)
{
    x_component = position.x;
    y_component = position.y;
}

QLength Vector2D::magnitude()
{
    QLength mag = std::sqrt(SQ(x_component.convert(meter)) + SQ(y_component.convert(meter))) * meter;
    return mag.abs();
}

QAngle Vector2D::direction()
{
    QAngle theta = std::atan2(y_component.convert(meter), x_component.convert(meter)) * radian;
    return constrainAngle360(theta);
}

QLength Vector2D::dist(Vector2D origin, Vector2D destination)
{
    double x_components = std::pow(destination.x_component.convert(meter) - origin.x_component.convert(meter), 2);
    double y_components = std::pow(destination.y_component.convert(meter) - origin.y_component.convert(meter), 2);
    return ((std::sqrt(x_components + y_components)) * meter);
}

QLength Vector2D::dist(Vector2D target)
{
    return Vector2D::dist(*this, target);
}

Vector2D Vector2D::normalize()
{
    Vector2D normalizedVector;
    QLength mag = magnitude();
    if(mag.convert(meter) != 0)
    { 
        normalizedVector.x_component =  QLength ((x_component.convert(meter) / mag.convert(meter)) * meter);
        normalizedVector.y_component =  QLength ((y_component.convert(meter) / mag.convert(meter)) * meter);
    }
    return normalizedVector;
}

double Vector2D::dot(Vector2D multiplicand)
{
    return (x_component.convert(meter) * multiplicand.x_component.convert(meter)) + (y_component.convert(meter) * multiplicand.y_component.convert(meter));
}