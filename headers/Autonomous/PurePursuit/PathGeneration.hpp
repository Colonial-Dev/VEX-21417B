#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"

RawPath injectPoints(RawPath path_outline, QLength spacing);

RawPath smoothPath(RawPath rough_path, GenerationParameters parameters);

Path processPath(RawPath smooth_path, RobotProperties robot, GenerationParameters params);

Path generatePath(RobotProperties robot_props, GenerationParameters parameters, std::vector<Vector> path_outline);