#pragma once
#include "CoordinateMath.h"
#include "VectorMath.h"
#define SQ(x) std::pow(x, 2)

RawPath injectPoints(RawPath path_outline, double spacing = 6.0)
{
    RawPath paddedPath;

    for(int i = 0; i < path_outline.size() - 1; i++)
    {
        RawPoint start_point = path_outline.at(i);
        RawPoint end_point = path_outline.at(i+1);

        Vector segment(start_point, end_point);
        int injectionCount = std::ceil(segment.magnitude() / spacing);
        segment = segment.normalize().scalarMult(spacing);
        
        for(int i = 0; i < injectionCount; i++)
        {
            Vector injectionVector = segment.scalarMult(i);
            RawPoint injectionPoint = start_point;

            injectionPoint.x_pos += injectionVector.x_component;
            injectionPoint.y_pos += injectionVector.y_component;

            paddedPath.add(injectionPoint);
        }
    }

    paddedPath.add(path_outline.at(path_outline.size() - 1));
    return paddedPath;
}

RawPath smoothPath(RawPath rough_path, double data_weight, double smooth_weight, double tolerance)
{
    double change = tolerance;
    RawPath smoothedPath = rough_path;

    while(change >= tolerance)
    {
        change = 0.0;
        for(int i = 1; i < rough_path.size() - 1; i++)
        {
            RawPoint smoothed_point = smoothedPath.at(i);
            RawPoint prev_point = smoothedPath.at(i-1);
            RawPoint next_point = smoothedPath.at(i+1);
            RawPoint rough_point = rough_path.at(i);
            RawPoint orig_point = {smoothed_point.x_pos, smoothed_point.y_pos};

            smoothed_point.x_pos += data_weight * (rough_point.x_pos - smoothed_point.x_pos) + smooth_weight * (prev_point.x_pos + next_point.x_pos - (2.0 * smoothed_point.x_pos));
            smoothed_point.y_pos += data_weight * (rough_point.y_pos - smoothed_point.y_pos) + smooth_weight * (prev_point.y_pos + next_point.y_pos - (2.0 * smoothed_point.y_pos));

            change += std::abs(orig_point.x_pos - smoothed_point.x_pos);
            change += std::abs(orig_point.y_pos - smoothed_point.y_pos);

            smoothedPath.at(i) = smoothed_point;
        }
    }

    return smoothedPath;
}

Path processPath(RawPath smooth_path, RobotProperties robot, GenerationParameters params)
{
    Path newPath;

    //Calculate point distances
    double prev_dist = 0.0;
    RawPoint prev_point = smooth_path.at(0);
    for(int i = 0; i < smooth_path.size(); i++)
    {
        RawPoint raw_point = smooth_path.at(i);
        PathPoint new_point = {raw_point.x_pos, raw_point.y_pos};
        new_point.distance = prev_dist + interpointDistance(prev_point, raw_point);

        prev_dist = new_point.distance;
        prev_point = raw_point;
        newPath.add(new_point);
    }

    //Calculate path curvature per point
    newPath.at(0).curvature = 0;
    newPath.at(newPath.size() - 1).curvature = 0;
    for(int i = 1; i < newPath.size() - 1; i++)
    {
        PathPoint current_point = newPath.at(i);
        PathPoint prev_point = newPath.at(i - 1);
        PathPoint next_point = newPath.at(i + 1);

        if(current_point.x_pos == prev_point.x_pos) { current_point.x_pos += 0.001; }

        double curr_x = current_point.x_pos;
        double curr_y = current_point.y_pos;
        double prev_x = prev_point.x_pos;
        double prev_y = prev_point.y_pos;
        double next_x = next_point.x_pos;
        double next_y = next_point.y_pos;

        double k_one = 0.5 * (SQ(curr_x) + SQ(curr_y) - SQ(prev_x) - SQ(prev_y)) / (curr_x - prev_x);

        double k_two = (curr_y - prev_y) / (curr_x - prev_x);

        double b = 0.5 * (SQ(prev_x) - 2 * prev_x * k_one + SQ(prev_y) - SQ(next_x) + 2 * next_x * k_one - SQ(next_y)) 
                         / (next_x * k_two - next_y + prev_y - prev_x * k_two);

        double a = k_one - k_two * b;
        
        double r = std::sqrt(SQ(curr_x - a) + SQ(curr_y - b));

        double curvature = 1/r; 
        if(std::isnan(curvature)) { curvature = 0; }
        newPath.at(i).curvature = curvature;
    }
    
    //Calculate initial max velocity per point
    for(int i = 0; i < newPath.size(); i++)
    {
        newPath.at(i).target_velocity = std::min(robot.max_velocity, params.initial_velocity_constant / newPath.at(i).curvature);
    }

    //Smooth deceleration velocities
    for(int i = newPath.size() - 2; i >= 0; i++)
    {
        PathPoint current_point = newPath.at(i);
        PathPoint next_point = newPath.at(i+1);
        double distance = interpointDistance(current_point, next_point);
        newPath.at(i).target_velocity = std::min(current_point.target_velocity, std::sqrt(SQ(next_point.target_velocity) + 2 * robot.max_acceleration * distance));
    }
    
    return newPath;
}

