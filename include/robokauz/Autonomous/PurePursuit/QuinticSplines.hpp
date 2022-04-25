#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

class QuinticPolynomial
{
    private:

        double coefficients[6];

    public:

        QuinticPolynomial(QLength component_start, QLength component_end, QSpeed velocity_start, QSpeed velocity_end);

        QLength calculateComponent(double component);
};

class QuinticSegmentGenerator
{
    private:

        //No instantiation for you
        QuinticSegmentGenerator() {}

    public:

        static std::vector<SplinePoint> interpolateSegment(SplinePoint start, SplinePoint end, double resolution, bool is_end);
};

class QuinticPathGenerator
{
    private:

        std::vector<SplinePoint> path_points;

        const double RESOLUTION;

        const double SMOOTHING;

        std::vector<SplinePoint> convertToSplinePoints(std::vector<Waypoint> points);

        QAngle angleBetweenPointsSpline(SplinePoint start, SplinePoint target);

        void calculateAngles();

        void generateVelocities();

        void generatePath();

    public:

        QuinticPathGenerator(std::vector<Waypoint> points, double resolution, double smoothing_constant);

        std::vector<PathPoint> getPath();
};