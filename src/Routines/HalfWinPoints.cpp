#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftHalfWP()
{
    //Start facing left with in-belt preloads. 
    //Immediately grab the ramp mogo and hurl preloads into it.
    //At the same time, curve out, grab the small neutral and retreat.

    wayfarer.buildPath("Round_LHWP", {4, 18_in, 30, 0.7})
        .withRobotProperties({0.25_mps, 1.4_mps, 2.5_mps2, 12.0_in, 4.125_in, drive_controller})
        .withOrigin()
        .withPoint({1_ft, 0_ft})
        .withPoint({5.5_ft, 4.5_ft})
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
}