#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

struct Waypoint
{
    QLength x_pos;
    QLength y_pos;
    QAngle heading;
};

class PathBuilder
{
    private:

        std::string path_name;

        GenerationParameters gen_params;

        RobotProperties robot_props;

        std::vector<Waypoint> path_waypoints;

        PathManager& calling_manager;

        bool hasPrealignment = false;

        bool computeReversed = false;

        bool doDebugDump = false;

        std::vector<squiggles::Pose> transformToCartesian(std::vector<Waypoint> waypoints);

        std::vector<squiggles::ProfilePoint> generateSplinePath(std::vector<squiggles::Pose> waypoints);

        std::vector<PathPoint> stripForExport(std::vector<squiggles::ProfilePoint> path);

        void calculateCurvatures(Path& path);

        void calculateVelocities(Path& path);

        Path calculatePath();

    public:

        PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller);

        PathBuilder withRobotProperties(RobotProperties r_props);

        PathBuilder withPoint(Waypoint point);

        PathBuilder withPoints(std::vector<Waypoint> points);

        PathBuilder withOrigin();

        PathBuilder withRotatedOrigin(QAngle rotation);

        PathBuilder withCurrentPosition(IMUOdometer& odometer);

        PathBuilder withPrealignment();

        PathBuilder makeReversed();

        PathBuilder withDebugDump();

        void generatePath();
};
