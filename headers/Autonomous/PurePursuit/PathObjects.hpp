#pragma once
#include "robokauz/PROS.hpp"
#include "../VectorMath.hpp"
#include "PathComponents.hpp"

/**
 * @brief Represents a raw (i.e. not fully generated/in progress) path.
 */
class RawPath
{
    private:

        /**
         * @brief A vector of Vectors, containing the (X,Y) points of the path in sequential order.
         */
        std::vector<Vector> points;

    public:

        /**
         * @brief Constructs a new Raw Path object with no information.
         */
        RawPath() {}

        /**
         * @brief Constructs a new Raw Path object from a provided set of points.
         * @param pts A vector of Vectors, containg the (X,Y) points of the path in sequential order.
         */
        RawPath(std::vector<Vector> pts);

        /**
         * @brief Returns the size of the path.
         */
        int size();

        /**
         * @brief Gets the Vector point at the specified index in the path.
         * @param index The index of the point to fetch.
         * @return A reference to the Vector point at the specified index.
         */
        Vector& at(int index);

        /**
         * @brief Adds a Vector point to the end of the path.
         * @param point The Vector point to add.
         */
        void add(Vector point);
};

/**
 * @brief Represents a full (i.e. fully generated/complete) path.
 */
class Path
{
    private:

        /**
         * @brief The name of the path.
         */
        std::string name;

        /**
         * @brief A vector of PathPoints, representing the (X,Y) points of the path and associated data in sequential order.
         */
        std::vector<PathPoint> points;

    public:

        /**
         * @brief Sets the path's name.
         * @param new_name The new name for the path.
         */
        void setName(std::string new_name);

        /**
         * @brief Get the path's name.
         */
        std::string getName();
        
        /**
         * @brief Returns the size of the path.
         */
        int size();

        /**
         * @brief Gets the point at the specified index in the path.
         * @param index The index of the point to fetch.
         * @return A reference to the point at the specified index.
         */
        PathPoint& at(int index);

        /**
         * @brief Adds a point to the end of the path.
         * @param point The point to add.
         */
        void add(PathPoint point);
};