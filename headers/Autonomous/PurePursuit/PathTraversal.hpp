#pragma once
#include "robokauz/PROS.hpp"
#include "../VectorMath.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"

struct TraversalParameters
{
    QLength lookahead_distance;
};

struct WheelSpeeds
{
    QAngularSpeed target_left;
    QAngularSpeed target_right;
};

struct TraversalCache
{
    TraversalParameters params;
    RobotProperties robot_properties;
    OdomState current_position;
    Path path;

    PathPoint closest_point;
    Vector lookahead_point;
    Vector projected_lookahead;
    int closest_index = 0;
    int lookahead_index = 0;

    WheelSpeeds target_speeds;
};

void updatePosition(TraversalCache& cache);

void updateClosestPoint(TraversalCache& cache);

double findIntersect(Vector& start, Vector& end, Vector& pos, double lookahead);

void updateLookaheadPoint(TraversalCache& cache);

void projectLookaheadPoint(TraversalCache& cache);

double calculateCurvature(TraversalCache& cache);

bool checkDistance(TraversalCache& cache, QLength threshold);

void calculateWheelSpeeds(TraversalCache &cache, double curvature);