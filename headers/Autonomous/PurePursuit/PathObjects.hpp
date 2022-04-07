#pragma once
#include "robokauz/PROS.hpp"
#include "../VectorMath.hpp"
#include "PathComponents.hpp"

class RawPath
{
    private:

        std::vector<Vector> points;

    public:

        RawPath() {}

        RawPath(std::vector<Vector> pts);

        int size();

        Vector& at(int index);

        void add(Vector point);
};

class Path
{
    private:

        std::string name;
        std::vector<PathPoint> points;

    public:

        void setName(std::string new_name);

        std::string getName();

        int size();

        PathPoint& at(int index);

        void add(PathPoint point);
};