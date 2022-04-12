#include "robokauz/PROS.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

RawPoint::RawPoint(Point point)
{
    x_pos = point.x;
    y_pos = point.y;
}

RawPoint::RawPoint(Vector vector)
{
    x_pos = vector.x_component;
    y_pos = vector.y_component;
}