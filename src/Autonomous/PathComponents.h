#pragma once

struct RobotProperties
{
    double max_velocity;
    double max_acceleration;
    double track_width;
};

struct RobotPosition
{
    double x_pos;
    double y_pos;
    double heading;
};

struct GenerationParameters
{
    double data_weight;
    double smooth_weight;
    double tolerance;
    double initial_velocity_constant;
};

struct RawPoint
{
    double x_pos;
    double y_pos;
};

struct PathPoint
{
    double x_pos;
    double y_pos;
    double distance;
    double target_velocity;
    double curvature;
};

class RawPath
{
    private:

        std::vector<RawPoint> points;
    
    public:

        int size() { return points.size(); }

        RawPoint& at(int index) { return points.at(index); } 

        void add(RawPoint point) { points.push_back(point); }
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

