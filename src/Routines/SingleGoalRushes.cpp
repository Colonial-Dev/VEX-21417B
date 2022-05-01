#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::rightSingleRush()
{
    //Zip out, grab small neutral, retreat
    wayfarer.buildPath("Round_RSR", DEFAULT_GEN_PARAMS)
        .withOrigin()
        .withPoint({4.5_ft, 0_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RSR");
    CLAMP_CLOSE
    pros::delay(100);
    SETBRK(HOLD)
    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_RSR_rev");
    TURNRELGOAL(80_deg)
    CLAMP_OPEN
    TURNREL(0_deg);
}

void AutonomousRoutines::leftSingleRush()
{
    //Zip out, grab small neutral, retreat
    wayfarer.buildPath("Round_LSR", DEFAULT_GEN_PARAMS)
        .withOrigin()
        .withPoint({4.5_ft, 2_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_LSR");
    CLAMP_CLOSE
    pros::delay(100);
    SETBRK(HOLD)
    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_LSR_rev");
    TURNRELGOAL(-80_deg)
    CLAMP_OPEN
    TURNREL(0_deg)
}