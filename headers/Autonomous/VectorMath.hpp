#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

class Vector
{
    public:

        QLength x_component;
        QLength y_component;

        Vector operator + (Vector addend) { return {x_component + addend.x_component, y_component + addend.y_component}; }

        Vector operator - (Vector subtrahend) { return {x_component - subtrahend.x_component, y_component - subtrahend.y_component}; }

        Vector operator * (double scalar) { return {x_component * scalar, y_component * scalar}; }

        Vector operator / (double scalar) { return {x_component / scalar, y_component / scalar}; }

        Vector();

        Vector(QLength x_comp, QLength y_comp);

        Vector(QLength magnitude, QAngle direction);

        Vector(PathPoint point);

        Vector(OdomState position);

        QLength magnitude();

        QAngle direction();

        QLength distance(Vector target);

        Vector normalize();

        double dot(Vector multiplicand);
};

QLength interpointDistance(Vector point_a, Vector point_b);