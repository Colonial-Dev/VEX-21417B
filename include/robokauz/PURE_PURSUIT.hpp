//Note: PURE_PURSUIT brings all header files related to pure pursuit into scope, as well as a few useful macro definitions and necessary forward class declarations.
#include "robokauz/Autonomous/PurePursuit/PathComponents.hpp"
#include "robokauz/Autonomous/PurePursuit/QuinticSplines.hpp"
#include "robokauz/Autonomous/PurePursuit/PathBuilder.hpp"
#include "robokauz/Autonomous/PurePursuit/PathTraverser.hpp"
#include "robokauz/Autonomous/PurePursuit/PathManager.hpp"
#define DEFAULT_GEN_PARAMS {1.5, 16_in, 30, 1.0}

//Needed to resolve circular dependencies.
//(Thanks, retarded arcane C++ include system. I miss C#.)
class PathBuilder;
class PathManager; 
