#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftDoubleRush()
{
    AutonomousRoutines::leftSingleRush();
    TURNRELGOAL(-70_deg)
    LOCK_OPEN
    CLAMP_OPEN
    TURNREL(0_deg);

    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_LDR", {2.5, 18_in, 30, 0.6})
        .withPoint({-0.5_ft, 0_ft})
        .withPoint({2_ft, 2_ft})
        .withPoint({4.5_ft, 4_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_LDR");
    CLAMP_CLOSE
    pros::delay(100);
    SETBRK(HOLD)

    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_LDR_rev");
    CLAMP_OPEN
}

void AutonomousRoutines::rightDoubleRush()
{
    AutonomousRoutines::rightSingleRush();
    TURNRELGOAL(70_deg)
    LOCK_OPEN
    CLAMP_OPEN
    TURNREL(0_deg);
    
    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_RDR", {4, 24_in, 30, 0.6})
        .withPoint({-1.5_ft, 0_ft})
        .withPoint({2_ft, -2_ft})
        .withPoint({5_ft, -4_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    CLAMP_OPEN
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RDR");
    CLAMP_CLOSE
    pros::delay(100);
    SETBRK(HOLD)

    arm_controller.setTargetAsync(20_deg);
    wayfarer.synchronousTraverse("Round_RDR_rev");
    CLAMP_OPEN
}