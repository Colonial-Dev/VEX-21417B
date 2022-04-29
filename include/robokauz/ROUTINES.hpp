#pragma once
#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "robokauz/CONTROLLERS.hpp"
#include "robokauz/PURE_PURSUIT.hpp"
#include "robokauz/Autonomous/Macrolang.hpp"

void selectRoutine(int side, int strategy);

namespace AutonomousRoutines
{
    void skills();

    void rightFullWP();

    void rightHalfWP();

    void rightSingleRush();

    void rightDoubleRush();

    void leftFullWP();

    void leftHalfWP();

    void leftSingleRush();

    void leftDoubleRush();
}