#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftSingleRush()
{
    //Zip out, grab small neutral, retreat
    wayfarer.buildPath("Round_LSR", {2.5, 18_in, 30, 1})
        .withOrigin()
        .withPoint({4.5_ft, 2_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_LSR");
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(100);
    SETBRK(HOLD)
    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_LSR_rev");
}

void AutonomousRoutines::rightSingleRush()
{
    //Zip out, grab small neutral, retreat
    wayfarer.buildPath("Round_RSR", {2.5, 18_in, 30, 1})
        .withOrigin()
        .withPoint({4.5_ft, 0_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RSR");
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(100);
    SETBRK(HOLD)
    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_RSR_rev");
}