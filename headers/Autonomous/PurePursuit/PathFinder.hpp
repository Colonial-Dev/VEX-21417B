#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "okapi/squiggles/squiggles.hpp"
#include "robokauz/Autonomous/VectorMath.hpp"
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/PathObjects.hpp"

struct Waypoint
{
    QLength x_pos;
    QLength y_pos;
    QAngle heading;
};

class PathFinder
{
    private:

        squiggles::Constraints constraints;
        
        squiggles::SplineGenerator generator;

        std::vector<squiggles::Pose> transformToCartesian(std::vector<Waypoint> waypoints);

        std::vector<squiggles::ProfilePoint> generateSplinePath(std::vector<squiggles::Pose> waypoints);

        std::vector<PathPoint> stripForExport(std::vector<squiggles::ProfilePoint> path);

    public:

        PathFinder(RobotProperties r_props);

        Path generatePath(std::string path_name, std::vector<Waypoint> waypoints);

};