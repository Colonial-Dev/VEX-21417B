#pragma once

void dumpRawPath(RawPath path)
{
    PRINT "\n-----\nDUMPING RAW PATH";

    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += to_string(path.at(i).x_component.convert(foot)) + ",";
        y_coords += to_string(path.at(i).y_component.convert(foot)) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");

    PRINT x_coords + y_coords + "\n-----";
}

void dumpFullPath(Path path)
{
    PRINT "\n-----\nDUMPING PATH: " + path.getName();

    std::string x_coords = "\n X [";
    std::string y_coords = "\n Y [";
    std::string distances =  "\n D [";
    std::string target_vels = "\n V [";
    std::string curvatures = "\n C [";

    for(int i = 0; i < path.size(); i++)
    {
        x_coords += to_string(path.at(i).x_pos.convert(foot)) + ",";
        y_coords += to_string(path.at(i).y_pos.convert(foot)) + ",";
        distances += to_string(path.at(i).distance.convert(foot)) + ",";
        target_vels += to_string(path.at(i).target_velocity.convert(mps)) + ",";
        curvatures += to_string(path.at(i).curvature) + ",";
    }

    x_coords.replace(x_coords.length() - 1, 1, "]");
    y_coords.replace(y_coords.length() - 1, 1, "]");
    distances.replace(distances.length() - 1, 1, "]");
    target_vels.replace(target_vels.length() - 1, 1, "]");
    curvatures.replace(curvatures.length() - 1, 1, "]");

    PRINT x_coords + y_coords + distances + target_vels + curvatures + "\n-----";
}