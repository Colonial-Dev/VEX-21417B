#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

/**
 * @brief Class used to represent a vector in the 2D coordinate plane.
 */
class Vector2D
{
    public:

        QLength x_component; /** The X component of the vector. */
        QLength y_component; /** The Y component of the vector. */
        
        /**
         * @brief Adds the components of two Vectors together.
         * @param addend The Vector2D to add.
         * @return A Vector2D with the combined components.
         */
        Vector2D operator + (Vector2D addend) { return {x_component + addend.x_component, y_component + addend.y_component}; }

        /**
         * @brief Subtracts the components of the first vector by those of the second vector.
         * @param subtrahend The Vector2D to subtract by.
         * @return A Vector2D with the subtracted components.
         */
        Vector2D operator - (Vector2D subtrahend) { return {x_component - subtrahend.x_component, y_component - subtrahend.y_component}; }

        /**
         * @brief Multiplies a vector's components by a scalar value.
         * @param scalar The scalar value to multiply by.
         * @return A Vector2D with the multiplied components.
         */
        Vector2D operator * (double scalar) { return {x_component * scalar, y_component * scalar}; }

        /**
        * @brief Divides a vector's components by a scalar value.
        * @param scalar The scalar value to divide by.
        * @return A Vector2D with the divided components.
        */
        Vector2D operator / (double scalar) { return {x_component / scalar, y_component / scalar}; }

        /**
         * @brief Constructs a Vector2D with zeroed component values.
         */
        Vector2D();

        /**
         * @brief Constructs a Vector2D with the given component values.
         * @param x_comp The X component of the new Vector2D.
         * @param y_comp The Y component of the new Vector2D.
         */
        Vector2D(QLength x_comp, QLength y_comp);

        /**
         * @brief Constructs a new Vector2D object with the given magnitude and direction.
         * @note Converts the provided magnitude and direction into equivalent X and Y components.
         * @param magnitude The magnitude of the new Vector2D.
         * @param direction The direction of the new Vector2D.
         */
        Vector2D(QLength magnitude, QAngle direction);

        /**
         * @brief Constructs a new Vector2D object from the (X,Y) position of a SplinePoint.
         * @param point A SplinePoint to convert into a Vector2D.
         */
        Vector2D(SplinePoint point);

        /**
         * @brief Constructs a new Vector2D object from the (X,Y) position of a PathPoint.
         * @param point A PathPoint to convert into a Vector2D.
         */
        Vector2D(PathPoint point);

        /**
         * @brief Constructs a new Vector2D object from the (X,Y) position of an OdomState.
         * @param point An OdomState to convert into a Vector2D.
         */
        Vector2D(OdomState position);

        /**
         * @brief Calculates the magnitude of the Vector2D.
         */
        QLength magnitude();

        /**
         * @brief Calculates the direction of the Vector2D.
         */
        QAngle direction();


        static QLength dist(Vector2D origin, Vector2D destination);

        /**
         * @brief Calculates the straight-line distance between the endpoints of two Vectors.
         * @param target The target Vector2D.
         */
        QLength dist(Vector2D target);

        /**
         * @brief Calculates a normalized version of the Vector2D.
         * @return A normalized copy of the Vector2D.
         */
        Vector2D normalize();

        /**
         * @brief Calculate the dot product of this and another Vector2D.
         * @param multiplicand The Vector2D to multiply by.
         * @return double The scalar dot product of the two Vectors.
         */
        double dot(Vector2D multiplicand);
};