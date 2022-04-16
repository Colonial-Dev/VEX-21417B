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
    std::string left_vels =  "\n Vr [";
    std::string right_vels = "\n Vl [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += std::to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += std::to_string(path.at(i).y_pos.convert(foot)) + ",";
        left_vels += std::to_string(path.at(i).left_velocity.convert(mps)) + ",";
        right_vels += std::to_string(path.at(i).left_velocity.convert(mps)) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");
    left_vels.replace(left_vels.length() - 1, 1, "]");
    right_vels.replace(right_vels.length() - 1, 1, "]");

    PRINT(x_coords + y_coords + left_vels + right_vels + "\n-----");
}