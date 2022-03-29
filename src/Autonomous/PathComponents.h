#pragma once

struct RobotProperties
{
    QSpeed max_velocity;
    QAcceleration max_acceleration;
    QLength track_width;
    QLength wheel_diam;
    double kv;
    double ka;
    double kp;
};

struct RobotPosition
{
    QLength x_pos;
    QLength y_pos;
    QAngle heading;
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
    QLength x_pos;
    QLength y_pos;
};

struct PathPoint
{
    QLength x_pos;
    QLength y_pos;
    QLength distance;
    QSpeed target_velocity;
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

struct WheelSpeeds
{
    QAngularSpeed target_left;
    QAngularSpeed target_right;
};

