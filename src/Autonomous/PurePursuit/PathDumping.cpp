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
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");

    PRINT(x_coords + y_coords + "\n-----");
}

void dumpFullPath(Path path)
{
    PRINT("\n-----\nDUMPING PATH: " + path.getName());

    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += std::to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += std::to_string(path.at(i).y_pos.convert(foot)) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");

    PRINT(x_coords + y_coords + "\n-----");
}