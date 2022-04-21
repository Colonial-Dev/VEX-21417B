#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/Autonomous/PurePursuit/PathDumping.hpp"

void dumpFullPath(Path path)
{
    PRINT("\n-----\nDUMPING PATH: " + path.name);

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