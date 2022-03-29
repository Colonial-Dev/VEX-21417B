#pragma once

QLength interpointDistance(RawPoint point_a, RawPoint point_b)
{
    double x_components = std::pow(point_b.x_pos.convert(inch) - point_a.x_pos.convert(inch), 2);
    double y_components = std::pow(point_b.y_pos.convert(inch) - point_a.y_pos.convert(inch), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * inch);
    return qlen;
}

QLength interpointDistance(PathPoint point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos.convert(inch) - point_a.x_pos.convert(inch), 2);
    double y_components = std::pow(point_b.y_pos.convert(inch) - point_a.y_pos.convert(inch), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * inch);
    return qlen;
}

QLength interpointDistance(RobotPosition point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos.convert(inch) - point_a.x_pos.convert(inch), 2);
    double y_components = std::pow(point_b.y_pos.convert(inch) - point_a.y_pos.convert(inch), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * inch);
    return qlen;
}