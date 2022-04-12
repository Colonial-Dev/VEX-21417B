#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "PathComponents.hpp"
#include "PathObjects.hpp"

/**
 * @brief Injects additional consistently-spaced points into a given path outline.
 * @param path_outline The path outline to inject points into.
 * @param spacing The spacing between injected points. 6 inches is a good default.
 * @return A RawPath with additional points injected according to the provided spacing value. 
 */
RawPath injectPoints(RawPath path_outline, QLength spacing);

/**
 * @brief Smooths out rough paths into a more curvilinear trajectory, where possible.
 * @param rough_path The rough path to smooth. Should be sourced by running an outline through injectPoints().
 * @param parameters A GenerationParameters structure. Contains configurable constants used in the smoothing algorithm.
 * @return A RawPath containing a smoothed version of the original rough trajectory.
 */
RawPath smoothPath(RawPath rough_path, GenerationParameters parameters);

/**
 * @brief Performs the final processing steps needed to convert a RawPath into a full Path.
 * @param smooth_path The smoothed RawPath to convert into a full Path. Should be sourced from smoothPath().
 * @param robot A RobotProperties structure. Contains information about robot dimensions used in final processing.
 * @param params A GenerationParameters structure. Contains a configurable division constant used in final processing.
 * @return A ready-to-traverse Path object. 
 */
Path processPath(RawPath smooth_path, RobotProperties robot, GenerationParameters params);

/**
 * @brief Generates a full Path from a RawPoint waypoint outline using injectPoints(), smoothPath() and processPath().
 * @param robot_props A correctly-configured RobotProperties structure.
 * @param parameters A configured GenerationParameters structure.
 * @param path_outline A std::vector of RawPoints, representing the outline of the path.
 * @return A ready-to-traverse Path object.
 */
Path generatePath(RobotProperties robot_props, GenerationParameters parameters, std::vector<RawPoint> path_outline);