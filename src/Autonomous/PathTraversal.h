#include <cfloat>
#pragma once

struct TraversalParameters
{
    double lookahead_distance;
};

struct TraversalCache
{
    TraversalParameters params;
    RobotProperties robot_properties;
    RobotPosition current_position;
    Path path;
    PathPoint closest_point;
    PathPoint lookahead_point;
    int closest_index;
    int lookahead_index;
};

double rateLimit()
{
    return 0.0;
}

void updateClosestPoint(TraversalCache& cache)
{
    double curr_closest_distance = DBL_MAX;
    int curr_closest_index = cache.closest_index;

    for(int i = curr_closest_index; i < cache.path.size(); i++)
    {
        double distance = interpointDistance(cache.current_position, cache.path.at(i));
        if(distance < curr_closest_distance)
        {
            curr_closest_distance = distance;
            curr_closest_index = i;
        }
    }

    cache.closest_index = curr_closest_index;
    cache.closest_point = cache.path.at(curr_closest_index);
}

void updateLookaheadPoint(TraversalCache& cache)
{

}