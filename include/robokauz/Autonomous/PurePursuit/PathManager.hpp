#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include <map>
#include <optional>

class PathManager
{
    private:

        RobotProperties robot_props;

        std::map<std::string, Path> stored_paths;

        std::optional<PathTraverser> current_traverser;

    public:

        PathManager(RobotProperties properties);

        PathBuilder buildPath(std::string name, GenerationParameters g_params);

        void insertPath(Path path);

        void deletePath(std::string path_name);

        void synchronousTraverse(std::string path_name);

        void asynchronousTraverse(std::string path_name);

        void waitUntilSettled();

        PathTraverser getTraverser(std::string path_name);

        void dumpPath(std::string path_name);
};