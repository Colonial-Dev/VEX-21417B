#pragma once
#include "robokauz/PROS.hpp"
#include "PathObjects.hpp"

/**
 * @brief Dumps a RawPath (i.e. not fully generated/in progress) object to std::cout. 
 * @param path The RawPath object to dump.
 */
void dumpRawPath(RawPath path);

/**
 * @brief Dumps a Path (i.e. fully generated/complete) object to std::cout.
 * @param path The Path object to dump.
 */
void dumpFullPath(Path path);