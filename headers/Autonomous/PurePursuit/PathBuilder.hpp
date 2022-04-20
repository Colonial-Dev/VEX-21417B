#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/IMUOdometry.hpp"

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

        Path calculatePath();

    public:

        PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller);

        PathBuilder withRobotProperties(RobotProperties r_props);

        PathBuilder withPoint(Waypoint point);

        PathBuilder withPoints(std::vector<Waypoint> points);

        PathBuilder withOrigin();

        PathBuilder withCurrentPosition(IMUOdometer& odometer);

        PathBuilder withPrealignment();

        PathBuilder makeReversed();

        PathBuilder withDebugDump();

        void generatePath();
};
