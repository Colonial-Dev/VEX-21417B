#pragma once
#include "VectorMath.h"
#define SQ(x) std::pow(x, 2);

RawPath injectPoints(RawPath path_outline, double spacing = 6.0)
{
    RawPath paddedPath;

    for(int i = 0; i < path_outline.points.size() - 1; i++)
    {
        RawPoint start_point = path_outline.at(i);
        RawPoint end_point = path_outline.at(i+1);
        Vector segment(start_point, end_point);
        int injectionCount = std::ceil(segment.magnitude() / spacing);
        segment = segment.normalize().scalarMult(spacing);
        
        for(int i = 0; i < injectionCount; i++)
        {
            Vector injectionVector = vector.scalarMult(i);
            RawPoint injectionPoint = start_point;
            injectionPoint.x_pos += injectionVector.x_component;
            injectionPoint.y_pos += injectionVector.y_component;
            paddedPath.points.push_back(injectionPoint);
        }
    }

    paddedPath.push_back(path_outline.at(path_outline.size() - 1));
    return paddedPath;
}

RawPath smoothPath(RawPath rough_path, double data_weight, double smooth_weight, double tolerance)
{
    double change = tolerance;
    RawPath smoothedPath = rough_path;

    while(change >= tolerance)
    {
        change = 0.0;
        for(int i = 1; i < rough_path.points.size() - 1; i++)
        {
            RawPoint smoothed_point = smoothedPath.points.at(i);
            RawPoint prev_point = smoothedPath.points.at(i-1);
            RawPoint next_point = smoothedPath.points.at(i+1);
            RawPoint rough_point = rough_path.points.at(i);
            RawPoint orig_point = {smoothed_point.x_pos, smoothed_point.y_pos};

            smoothed_point += data_weight * (rough_point.x_pos - smoothed_point.x_pos) + smooth_weight * (prev_point.x_pos + next_point.x_pos - (2.0 * smoothed_point.x_pos));
            smoothed_point += data_weight * (rough_point.y_pos - smoothed_point.y_pos) + smooth_weight * (prev_point.y_pos + next_point.y_pos - (2.0 * smoothed_point.y_pos));

            change += std::abs(orig_point.x_pos - smoothed_point.x_pos);
            change += std::abs(orig_point.y_pos - smoothed_point.y_pos;)
            smoothedPath.points.at(i) = smoothed_point;
        }
    }

    return smoothedPath;
}

double interpointDistance(RawPoint point_a, RawPoint point_b)
{
    double x_components = std::pow(point_b.x_pos - point_a.x_pos, 2);
    double y_components = std::pow(point_b.y_pos - point_a.y_pos, 2);
    return std::sqrt(x_components + y_components);
}

Path processPath(RawPath smooth_path)
{
    Path newPath;

    //Calculate point distances
    double prev_dist = 0.0;
    RawPoint prev_point = smooth_path.points.at(0);
    for(int i = 0; i < smooth_path.points.size(); i++)
    {
        RawPoint raw_point = smooth_path.points.at(i);
        PathPoint new_point = {raw_point.x_pos, raw_point.y_pos};
        new_point.distance = prev_dist + interpointDistance(prev_point, raw_point);

        prev_dist = new_point.distance;
        prev_point = raw_point;
        newPath.points.push_back(new_point);
    }

    //Calculate path curvature per point
    newPath.points.at(0).curvature = 0;
    newPath.points.at(newPath.points.size() - 1).curvature = 0;
    for(int i = 1; i < newPath.points.size() - 1; i++)
    {
        PathPoint current_point = newPath.points.at(i);
        PathPoint prev_point = newPath.points.at(i - 1);
        PathPoint next_point = newPath.points.at(i + 1);

        if(current_point.x_pos == prev_point.x_pos) { current_point.x_pos += 0.001; }

        double k_one = 0.5 * (SQ(current_point.x_pos) + SQ(current_point.y_pos) - SQ(prev_point.x_pos) - SQ(prev_point.y_pos)) / (current_point.x_pos - prev_point.x_pos);
        double k_two = (current_point.y_pos - prev_point.y_pos) / (current_point.x_pos - prev_point.x_pos);
        double b = 0.5 * (SQ(prev_point.x_pos) - 2 * prev_point.x_pos * k_one + SQ(prev_point.y_pos) - SQ(next_point.x_pos) + 2 * next_point.x_pos * k_one - SQ(next_point.y_pos)) 
                         / (next_point.x_pos * k_two - next_point.y_pos + prev_point.y_pos - prev_point.x_pos * k_two);
        double a = k_one - k_two * b;
        double r = std::sqrt(SQ(current_point.x_pos - a) + SQ(current_point.y_pos - b));

        double curvature = 1/r; 
        if(curvature == nan) { curvature = 0; }
        newPath.points.at(i).curvature = curvature;
    }
    
    //Calculate max velocity per point
}

