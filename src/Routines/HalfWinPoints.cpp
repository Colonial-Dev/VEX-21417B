#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftHalfWP()
{
    //Start facing left with in-belt preloads. 
    //Immediately grab the ramp mogo and hurl preloads into it.
    //At the same time, curve out, grab the small neutral and retreat.
    wayfarer.buildPath("Round_LHWP", {4, 18_in, 30, 1.0})
        .withOrigin()
        .withPoint({1.3_ft, -1.0_ft})
        .withPoint({5.5_ft, -0.1_ft})
        .makeReversed()
        .generatePath();
    
    SETROT(-90_deg) //We start facing left
    arm_controller.setTargetAsync(30_deg);
    CONVEYOR_ON

    CLIP_CLOSE //Clip mogo

    //Swerve out and coast into + grab small neutral
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_LHWP");
    CLAMP_UNFOLD
    DELAY(3500_ms)
    arm_controller.setTargetAsync(0_deg);
    wayfarer.waitUntilSettled();
    CONVEYOR_OFF

    CLAMP_CLOSE
    LOCK_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    arm_controller.setTargetAsync(30_deg);
    wayfarer.synchronousTraverse("Round_LHWP_rev");
}

void AutonomousRoutines::rightHalfWP()
{
    //Start facing forwards with in-belt preloads.
    //Zip out and grab the small neutral.
    //Curve backwards and clip the line mogo. Hurl preloads into it.
    //Retreat into scoring zone at the same time.

    wayfarer.buildPath("Round_RHWP_1", {2.5, 18_in, 30, 1})
        .withOrigin()
        .withPoint({4.5_ft, 0_ft})
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_RHWP_1");
    CLAMP_UNFOLD
    CLIP_OPEN
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(100);
    SETBRK(HOLD)

    arm_controller.setTargetAsync(30_deg);

    wayfarer.buildPath("Round_RHWP_2", {2.5, 18_in, 30, 0.75})
        .withCurrentPosition(imu_odometer)
        .withPoint({3.5_ft, 0_ft})
        .withPoint({2.5_ft, 1_ft})
        .isReversed()
        .generatePath();
    
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RHWP_2");
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    CONVEYOR_ON
    TURNREL(0_deg)
    wayfarer.traverseDistance(2.5_ft);
    wayfarer.traverseDistance(-4_ft);
}