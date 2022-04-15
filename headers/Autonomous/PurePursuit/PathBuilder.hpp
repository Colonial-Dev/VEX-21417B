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

        QLength current_lookahead = 12_in;

        std::vector<RawPoint> current_component;

        std::vector<std::vector<RawPoint>> path_components;

        PathManager& calling_manager;

        bool hasPrealignment = false;

        bool computeReversed = false;

        bool doDebugDump = false;

        void finalizeCurrentComponent();

    public:

        PathBuilder(std::string name, GenerationParameters g_params, RobotProperties r_props, PathManager& caller);

        PathBuilder withRobotProperties(RobotProperties r_props);

        PathBuilder withPoint(Point point);

        PathBuilder withPoints(std::vector<Point> points);

        PathBuilder withOrigin();

        PathBuilder withCurrentPosition(IMUOdometer& odometer);

        PathBuilder withLookahead(QLength lookahead);

        PathBuilder segment();

        PathBuilder linkedSegment();

        PathBuilder withPrealignment();

        PathBuilder makeReversed();

        PathBuilder withDebugDump();

        void generatePath();
};
