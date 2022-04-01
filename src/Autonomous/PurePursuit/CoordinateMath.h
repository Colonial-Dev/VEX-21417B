#pragma once

QLength interpointDistance(Vector point_a, Vector point_b)
{
    double x_components = std::pow(point_b.x_component.convert(meter) - point_a.x_component.convert(meter), 2);
    double y_components = std::pow(point_b.y_component.convert(meter) - point_a.y_component.convert(meter), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * meter);
    return qlen;
}

QLength interpointDistance(PathPoint point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos.convert(meter) - point_a.x_pos.convert(meter), 2);
    double y_components = std::pow(point_b.y_pos.convert(meter) - point_a.y_pos.convert(meter), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * meter);
    return qlen;
}

QLength interpointDistance(OdomState point_a, PathPoint point_b)
{
    double x_components = std::pow(point_b.x_pos.convert(meter) - point_a.x.convert(meter), 2);
    double y_components = std::pow(point_b.y_pos.convert(meter) - point_a.y.convert(meter), 2);
    QLength qlen = ((std::sqrt(x_components + y_components)) * meter);
    return qlen;
}