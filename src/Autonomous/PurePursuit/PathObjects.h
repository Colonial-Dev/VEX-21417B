#pragma once
#include "Autonomous/VectorMath.h""

class RawPath
{
    private:

        std::vector<Vector> points;
    
    public:

        RawPath()
        {
            
        }

        RawPath(std::vector<Vector> pts)
        {
            points = pts;
        }

        int size() { return points.size(); }

        Vector& at(int index) { return points.at(index); } 

        void add(Vector point) { points.push_back(point); }
};

class Path
{
    private:

        std::string name;
        std::vector<PathPoint> points;

    public:

        void setName(std::string new_name) { name = new_name; }

        std::string getName() { return name; }

        int size() { return points.size(); }

        PathPoint& at(int index) { return points.at(index); } 

        void add(PathPoint point) { points.push_back(point); }
};