#pragma once

struct RobotProperties
{
    double max_velocity;
    double max_acceleration;
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

struct RawPath
{
    std::vector<RawPoint> points;
};

struct Path
{
    std::string name;
    std::vector<PathPoint> points;
};

