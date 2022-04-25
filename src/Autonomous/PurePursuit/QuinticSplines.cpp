#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/QuinticSplines.hpp"

//Note: this is all mathematical voodoo I blatantly cribbed/translated from someone's Javascript code. IDK how it works but it does.

// ---- QuinticPolynomial ---

QuinticPolynomial::QuinticPolynomial(QLength component_start, QLength component_end, QSpeed velocity_start, QSpeed velocity_end)
{
    const double U = (component_end - component_start).convert(meter) - velocity_start.convert(mps);
    const double V = (velocity_end - velocity_start).convert(mps);
    const double A3 = 10.0 * U - 4.0 * V;
    const double A4 = -15.0 * U + 7.0 * V;
    const double A5 = 6.0 * U - 3.0 * V;
    double new_coefficients[6] = {component_start.convert(meter), velocity_start.convert(mps), 0.0, A3, A4, A5};
    std::copy(new_coefficients, new_coefficients + 6, coefficients);
}

QLength QuinticPolynomial::calculateComponent(double component)
{
    double xT = 0.0;

    for(int power = 0; power < 6; power++)
    {
        xT += coefficients[power] * std::pow(component, power);
    }

    return xT * meter;
}

// --- QuinticSegmentGenerator --

std::vector<SplinePoint> QuinticSegmentGenerator::interpolateSegment(SplinePoint start, SplinePoint end, double resolution, bool is_end)
{
    std::vector<SplinePoint> interpolated_segment;

    const QSpeed VXS = start.velocity.convert(mps) * std::sin(start.heading.convert(radian)) * mps;
    const QSpeed VYS = start.velocity.convert(mps) * std::cos(start.heading.convert(radian)) * mps;
    const QSpeed VXG = end.velocity.convert(mps) * std::sin(end.heading.convert(radian)) * mps;
    const QSpeed VYG = end.velocity.convert(mps) * std::cos(end.heading.convert(radian)) * mps;

    QuinticPolynomial x_polynomial(start.x_pos, end.x_pos, VXS, VXG);
    QuinticPolynomial y_polynomial(start.y_pos, end.y_pos, VYS, VYG);

    for(int i = 0; i <= (is_end ? resolution : resolution - 1); i++)
    {
        QLength x_component = x_polynomial.calculateComponent(i / resolution);
        QLength y_component = y_polynomial.calculateComponent(i / resolution);
        interpolated_segment.push_back({x_component, y_component, 0_deg, 0_mps});
    }

    return interpolated_segment;
}

// --- QuinticPathGenerator ---

QuinticPathGenerator::QuinticPathGenerator(std::vector<Waypoint> points, double resolution, double smoothing_constant) : RESOLUTION(resolution), SMOOTHING(smoothing_constant)
{
    path_points = convertToSplinePoints(points);
    calculateAngles();
    generateVelocities();
    generatePath();
}

std::vector<SplinePoint> QuinticPathGenerator::convertToSplinePoints(std::vector<Waypoint> points)
{
    std::vector<SplinePoint> converted_points;

    for(int i = 0; i < points.size(); i++)
    {
        converted_points.push_back({points.at(i).x_pos, points.at(i).y_pos, 0_deg, 0_mps});   
    }

    return converted_points;
}

QAngle QuinticPathGenerator::angleBetweenPointsSpline(SplinePoint start, SplinePoint target)
{
    return rollAngle180(std::atan2( (target.x_pos - start.x_pos).convert(meter), (target.y_pos - start.y_pos).convert(meter) ) * radian);
}

void QuinticPathGenerator::calculateAngles()
{
    path_points.at(0).heading = angleBetweenPointsSpline(path_points.at(0), path_points.at(1));

    for(int i = 1; i < path_points.size() - 1; i++)
    {
        path_points.at(i).heading = angleBetweenPointsSpline(path_points.at(i - 1), path_points.at(i + 1));
    }

    path_points.at(path_points.size() - 1).heading = angleBetweenPointsSpline(path_points.at(path_points.size() - 2), path_points.at(path_points.size() - 1));
}

void QuinticPathGenerator::generateVelocities()
{
    for(int i = 0; i < path_points.size() - 1; i++)
    {
        SplinePoint start_point = path_points.at(i);
        SplinePoint end_point = path_points.at(i + 1);
        double velocity = SMOOTHING * interpointDistance(start_point, end_point).convert(meter);

        path_points.at(i).velocity = velocity * mps;

        if(i == path_points.size() - 2)
        {
            path_points.at(i + 1).velocity = velocity * mps;
        }
    }
}

void QuinticPathGenerator::generatePath()
{
    if(path_points.size() == 2)
    {
        path_points = QuinticSegmentGenerator::interpolateSegment(path_points.at(0), path_points.at(1), RESOLUTION, true);
        return;
    }
    
    std::vector<SplinePoint> interpolated_path;

    for(int i = 0; i < path_points.size() - 1; i++)
    {
        SplinePoint start = path_points.at(i);
        SplinePoint end = path_points.at(i + 1);
        std::vector<SplinePoint> segment = QuinticSegmentGenerator::interpolateSegment(start, end, RESOLUTION, i >= path_points.size() - 2);
        interpolated_path.insert(interpolated_path.end(), segment.begin(), segment.end());    
    }

    path_points = interpolated_path;
}

std::vector<PathPoint>  QuinticPathGenerator::getPath()
{
    std::vector<PathPoint> stripped_path;

    for(int i = 0; i < path_points.size(); i++)
    {
        stripped_path.push_back({path_points.at(i).x_pos, path_points.at(i).y_pos, 0_mps, 0});
    }

    return stripped_path;
}
