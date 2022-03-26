#pragma once

double interpointDistance(RawPoint point_a, RawPoint point_b)
{
    double x_components = std::pow(point_b.x_pos - point_a.x_pos, 2);
    double y_components = std::pow(point_b.y_pos - point_a.y_pos, 2);
    return std::sqrt(x_components + y_components);
}

double interpointDistance(PathPoint point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos - point_a.x_pos, 2);
    double y_components = std::pow(point_b.y_pos - point_a.y_pos, 2);
    return std::sqrt(x_components + y_components);
}

double interpointDistance(RobotPosition point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos - point_a.x_pos, 2);
    double y_components = std::pow(point_b.y_pos - point_a.y_pos, 2);
    return std::sqrt(x_components + y_components);
}