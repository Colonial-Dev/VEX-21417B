#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftHalfWP()
{
    //Start facing left with in-belt preloads. 
    //Immediately grab the ramp mogo and hurl preloads into it.
    //At the same time, curve out, grab the small neutral and retreat.
    wayfarer.buildPath("Round_LHWP", {3, 18_in, 30, 1.0})
        .withOrigin()
        .withPoint({1.0_ft, -1.0_ft})
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

    while(imu_odometer.getPosition().x < 2.5_ft)
    {
        DELAY(10_ms)
    }

    arm_controller.setTargetAsync(0_deg);
    wayfarer.waitUntilSettled();
    CONVEYOR_OFF

    CLAMP_CLOSE
    LOCK_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    arm_controller.setTargetAsync(40_deg);
    wayfarer.synchronousTraverse("Round_LHWP_rev");
    CLIP_OPEN
}

void AutonomousRoutines::rightHalfWP()
{
    //Start facing forwards with in-belt preloads.
    //Zip out and grab the small neutral.
    //Curve backwards and clip the line mogo. Hurl preloads into it.
    //Retreat into scoring zone at the same time.

    wayfarer.buildPath("Round_RHWP_1", {2.5, 18_in, 30, 1})
        .withOrigin()
        .withPoint({4.25_ft, 0_ft})
        .generatePath();
    
    arm_controller.setTargetAsync(0_deg);
    
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_RHWP_1");
    CLAMP_UNFOLD
    wayfarer.waitUntilSettled();
    CLAMP_CLOSE
    LOCK_CLOSE
    pros::delay(250);
    SETBRK(HOLD)

    arm_controller.setTarget(30_deg);

    wayfarer.buildPath("Round_RHWP_2", {2.5, 16_in, 30, 0.75})
        .withPoint({0.75_ft, 2.95_ft})
        .withPoint({3.25_ft, 0_ft})
        .withCurrentPosition(imu_odometer)
        .makeReversed()
        .generatePath();
    
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Round_RHWP_2_rev");
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    CONVEYOR_ON
    wayfarer.traverseDistance(0.5_ft);
    TURNREL(0_deg)
    SETBRK(COAST)
    wayfarer.traverseDistance(2.5_ft);
    DELAY(500_ms)
    wayfarer.traverseDistance(-3.5_ft);
    CONVEYOR_OFF
    SETBRK(HOLD)
}