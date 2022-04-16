#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

/**
 * @brief Class used to represent a Vector in the 2D coordinate plane.
 */
class Vector
{
    public:

        QLength x_component; /** The X component of the vector. */
        QLength y_component; /** The Y component of the vector. */
        
        /**
         * @brief Adds the components of two Vectors together.
         * @param addend The Vector to add.
         * @return A Vector with the combined components.
         */
        Vector operator + (Vector addend) { return {x_component + addend.x_component, y_component + addend.y_component}; }

        /**
         * @brief Subtracts the components of the first vector by those of the second vector.
         * @param subtrahend The Vector to subtract by.
         * @return A Vector with the subtracted components.
         */
        Vector operator - (Vector subtrahend) { return {x_component - subtrahend.x_component, y_component - subtrahend.y_component}; }

        /**
         * @brief Multiplies a vector's components by a scalar value.
         * @param scalar The scalar value to multiply by.
         * @return A Vector with the multiplied components.
         */
        Vector operator * (double scalar) { return {x_component * scalar, y_component * scalar}; }

        /**
        * @brief Divides a vector's components by a scalar value.
        * @param scalar The scalar value to divide by.
        * @return A Vector with the divided components.
        */
        Vector operator / (double scalar) { return {x_component / scalar, y_component / scalar}; }

        /**
         * @brief Constructs a Vector with zeroed component values.
         */
        Vector();

        /**
         * @brief Constructs a Vector with the given component values.
         * @param x_comp The X component of the new Vector.
         * @param y_comp The Y component of the new Vector.
         */
        Vector(QLength x_comp, QLength y_comp);

        /**
         * @brief Constructs a new Vector object with the given magnitude and direction.
         * @note Converts the provided magnitude and direction into equivalent X and Y components.
         * @param magnitude The magnitude of the new Vector.
         * @param direction The direction of the new Vector.
         */
        Vector(QLength magnitude, QAngle direction);

        /**
         * @brief Constructs a new Vector object from the (X,Y) position of a RawPoint.
         * @param point A RawPoint to convert into a Vector.
         */
        Vector(RawPoint point);

        /**
         * @brief Constructs a new Vector object from the (X,Y) position of a PathPoint.
         * @param point A PathPoint to convert into a Vector.
         */
        Vector(PathPoint point);

        /**
         * @brief Constructs a new Vector object from the (X,Y) position of an OdomState.
         * @param point An OdomState to convert into a Vector.
         */
        Vector(OdomState position);

        /**
         * @brief Calculates the magnitude of the Vector.
         */
        QLength magnitude();

        /**
         * @brief Calculates the direction of the Vector.
         */
        QAngle direction();

        QAngle angleFrom(OdomState position);

        /**
         * @brief Calculates the straight-line distance between the endpoints of two Vectors.
         * @param target The target Vector.
         */
        QLength distance(Vector target);

        /**
         * @brief Calculates a normalized version of the Vector.
         * @return A normalized copy of the Vector.
         */
        Vector normalize();

        /**
         * @brief Calculate the dot product of this and another Vector.
         * @param multiplicand The Vector to multiply by.
         * @return double The scalar dot product of the two Vectors.
         */
        double dot(Vector multiplicand);
};

/**
 * @brief Calculate the straight-line distance between two Vector endpoints (or Vector-equivalent objects.)
 * @param point_a The first Vector.
 * @param point_b The second Vector.
 */
QLength interpointDistance(Vector point_a, Vector point_b);