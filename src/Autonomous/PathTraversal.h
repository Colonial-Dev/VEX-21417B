#pragma once

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