#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftMiddleRush()
{
    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_LDR", {4, 24_in, 30, 0.7})
        .withOrigin()
        .withPoint({0.5_ft, -1.0_ft})
        .withPoint({1.5_ft, 1.0_ft})
        .withPoint({5.5_ft, 3_ft})
        .makeReversed()
        .generatePath();
    
    SETROT(-90_deg)
    CLIP_CLOSE
    arm_controller.setTargetAsync(0_deg);
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_LDR");
    CLAMP_UNFOLD
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(250);
    SETBRK(HOLD)

    arm_controller.setTargetAsync(40_deg);
    wayfarer.synchronousTraverse("Round_LDR_rev");
    CLIP_OPEN
}

void AutonomousRoutines::rightMiddleRush()
{    
    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_RDR", {4, 18_in, 30, 0.6})
        .withPoint({-1.5_ft, 0_ft})
        .withPoint({3_ft, -2_ft})
        .withPoint({5_ft, -3.5_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_RDR");
    CLAMP_UNFOLD
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(250);
    SETBRK(HOLD)
    arm_controller.setTarget(20_deg);
    arm_controller.setTarget(20_deg);
    TURNREL(0_deg)

    wayfarer.buildPath("Round_RDR_2", {4, 17_in, 30, 0.6})
        .withPoint({-0.5_ft, 2.25_ft})
        .withPoint({3_ft, 0_ft})
        .withPoint({3.5_ft, -1_ft})
        .withPoint({5_ft, -3.5_ft})
        .makeReversed()
        .generatePath();

    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RDR_2_rev");
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    CONVEYOR_ON
    wayfarer.traverseDistance(1.5_ft);
    TURNREL(0_deg)
    arm_controller.setTarget(40_deg);
    wayfarer.traverseDistance(2.5_ft);
    wayfarer.traverseDistance(-3_ft);
    wayfarer.traverseDistance(0.5_ft);
    CLIP_OPEN
    CONVEYOR_OFF
}