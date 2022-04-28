#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"

enum ArmingState
{
    Disarmed,
    Armed,
    Delaying
};

enum RearmMode
{
    Never,
    Forever,
    Delayed
};

enum TriggerMode
{
    Instant,
    Threshold
};

class Sentinel
{

};