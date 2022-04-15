#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathDumping.hpp"

void dumpRawPath(RawPath path)
{
    PRINT("\n-----\nDUMPING RAW PATH");

    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";
    std::string lookaheads = "\n L [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += std::to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += std::to_string(path.at(i).y_pos.convert(foot)) + ",";
        lookaheads += std::to_string(path.at(i).lookahead_distance.convert(inch)) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");
    lookaheads.replace(lookaheads.length() - 1, 1, "]");

    PRINT(x_coords + y_coords + lookaheads + "\n-----");
}

void dumpFullPath(Path path)
{
    PRINT("\n-----\nDUMPING PATH: " + path.getName());

    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";
    std::string distances =  "\n D [";
    std::string lookaheads = "\n L [";
    std::string target_vels = "\n V [";
    std::string curvatures = "\n C [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += std::to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += std::to_string(path.at(i).y_pos.convert(foot)) + ",";
        distances += std::to_string(path.at(i).absolute_distance.convert(foot)) + ",";
        lookaheads += std::to_string(path.at(i).lookahead_distance.convert(inch)) + ",";
        target_vels += std::to_string(path.at(i).target_velocity.convert(mps)) + ",";
        curvatures += std::to_string(path.at(i).curvature) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");
    distances.replace(distances.length() - 1, 1, "]");
    lookaheads.replace(lookaheads.length() - 1, 1, "]");
    target_vels.replace(target_vels.length() - 1, 1, "]");
    curvatures.replace(curvatures.length() - 1, 1, "]");

    PRINT(x_coords + y_coords + distances + lookaheads + target_vels + curvatures + "\n-----");
}