#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/PathBuilder.hpp"
#include "robokauz/Autonomous/PurePursuit/PathTraverser.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManager.hpp"

//Needed to resolve circular dependencies.
//(Thanks, retarded arcane C++ include system. I miss C#.)
class PathBuilder;
class PathManager; 
class PathTraverser;
