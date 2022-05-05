#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftFullWP()
{
    //Start facing left with in-belt preloads. 
    //Immediately grab the ramp mogo and hurl preloads into it.
    //At the same time, curve out and head right along the ringle star line.
    //Drop the ramp mogo somewhere along the way. Try to keep some ringles in the belt.
    //Point-turn and back into the line mogo. Deposit ringles then retreat to scoring zone.

    wayfarer.buildPath("Round_LFWP", {4, 18_in, 30, 0.75})
        .withOrigin()
        .withPoint({1.3_ft, -1.0_ft})
        .withPoint({3_ft, 2_ft})
        .withPoint({3_ft, 4_ft})
        .withPoint({3_ft, 8_ft})
        .makeReversed()
        .generatePath();
    
    SETROT(-90_deg) //We start facing left
    arm_controller.setTargetAsync(30_deg);
    CONVEYOR_ON

    CLIP_CLOSE //Clip mogo

    //Swerve out and run through the ringle stars, dropping off the first mogo after a delay.
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_LFWP");
    CLAMP_UNFOLD
    DELAY(2500_ms)
    CLIP_OPEN
    CONVEYOR_OFF
    wayfarer.waitUntilSettled();

    TURNREL(-60_deg)
    SETBRK(COAST)
    wayfarer.traverseLinear({2_ft, 9_ft}, true);
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    CONVEYOR_ON
    wayfarer.traverseDistance(0.5_ft);
    TURNREL(0_deg)
    wayfarer.traverseDistance(2_ft);
    wayfarer.traverseDistance(-4_ft);
}

void AutonomousRoutines::rightFullWP()
{   
    //Start facing forwards with in-belt preloads.
    //Zip out and grab the small neutral.
    //Curve backwards and clip the line mogo. Hurl preloads into it.
    //Head left along the ringle start line, droppning the line mogo along the way. 
    //Overshoot the end of the balance and drop the small neutral.
    //Point-turn to face away from the ramp mogo. Back in and clip it, and add ringles.

    //For the moment, we'll just redirect this to the half win point routine.
    //This is primarily because the chances of our alliance partner NOT being in the way are damn near zero.
    AutonomousRoutines::rightHalfWP();
}