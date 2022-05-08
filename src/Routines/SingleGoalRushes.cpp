#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftSingleRush()
{
    //Zip out, grab small neutral, retreat
    wayfarer.buildPath("Round_LSR", {2.5, 16_in, 30, 1})
        .withOrigin()
        .withPoint({4.2_ft, 1_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_LSR");
    CLAMP_UNFOLD
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(250);
    SETBRK(HOLD)

    arm_controller.setTargetAsync(40_deg);
    wayfarer.buildPath("Round_LSR_2", {2.5, 16_in, 30, 1})
        .withPoint({-1_ft, 1.5_ft})
        .withPoint({2_ft, 0_ft})
        .withCurrentPosition(imu_odometer)
        .makeReversed()
        .generatePath();
    
    wayfarer.synchronousTraverse("Round_LSR_2_rev");
    CLIP_CLOSE
    CONVEYOR_ON
    DELAY(1000_ms)
    wayfarer.traverseDistance(0.5_ft);
    CLIP_OPEN
    CONVEYOR_OFF
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
    
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_RSR");
    CLAMP_UNFOLD
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(100);
    SETBRK(HOLD)
    arm_controller.setTargetAsync(40_deg);
    wayfarer.synchronousTraverse("Round_RSR_rev");
}