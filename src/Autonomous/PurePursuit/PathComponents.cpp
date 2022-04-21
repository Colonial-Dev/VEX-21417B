#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"

int Path::size() { return points.size(); }
PathPoint& Path::at(int index) { return points.at(index); } 
PathPoint& Path::start() { return points.at(0); }
PathPoint& Path::end() { return points.at(points.size() - 1); }
void Path::add(PathPoint point) { points.push_back(point); }