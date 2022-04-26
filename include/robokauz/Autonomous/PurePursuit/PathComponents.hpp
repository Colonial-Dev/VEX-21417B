#pragma once
#include "robokauz/PRELUDE.hpp"

struct RobotProperties
{   
    QSpeed min_velocity;
    QSpeed max_velocity;
    QAcceleration max_acceleration;
    QLength track_width;
    QLength wheel_diam;
    std::shared_ptr<OdomChassisController> odom_controller;
};

struct GenerationParameters
{
    double initial_velocity_constant; //Higher = faster
    QLength lookahead_distance;
    int spline_resolution;
    double smoothing_constant;
};

struct Waypoint
{
    QLength x_pos;
    QLength y_pos;
    QAngle heading = 0_deg;
};

struct SplinePoint
{
    QLength x_pos;
    QLength y_pos;
    QAngle heading;
    QSpeed velocity;
};

struct PathPoint
{
    QLength x_pos;
    QLength y_pos;
    QSpeed velocity = 0_mps;
    double curvature = 0;
};

/**
 * @brief Represents a full (i.e. fully generated/complete) path.
 */
class Path
{
    public:

        std::string name;

        std::vector<PathPoint> points;

        QLength lookahead_distance;

        double k_constant;

        bool reversed = false;

        bool prealign = false;
        
        int size();

        PathPoint& at(int index);

        PathPoint& start();

        PathPoint& end();

        void add(PathPoint point);
};