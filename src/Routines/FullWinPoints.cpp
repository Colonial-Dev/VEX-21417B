#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftFullWP()
{
    //Start facing left with in-belt preloads. 
    //Immediately grab the ramp mogo and hurl preloads into it.
    //At the same time, curve out and head right along the ringle star line.
    //Drop the ramp mogo somewhere along the way. Try to keep some ringles in the belt.
    //Point-turn and back into the line mogo. Deposit ringles then retreat to scoring zone.

    wayfarer.buildPath("Round_LFWP", {2, 18_in, 30, 0.75})
        .withOrigin()
        .withPoint({1.0_ft, 0_ft})
        .withPoint({3_ft, 2_ft})
        .withPoint({3_ft, 4_ft})
        .withPoint({3_ft, 7.5_ft})
        .makeReversed()
        .generatePath();
    
    SETROT(-90_deg) //We start facing left
    arm_controller.setTargetAsync(50_deg);
    CONVEYOR_ON

    CLIP_CLOSE //Clip mogo

    //Swerve out and run through the ringle stars, dropping off the first mogo once we've gone far enough on the Y axis.
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_LFWP");
    CLAMP_UNFOLD
    while (imu_odometer.getPosition().y < 3.5_ft)
    {
        DELAY(10_ms);
    }
    CLIP_OPEN
    CONVEYOR_OFF
    wayfarer.waitUntilSettled();

    TURNREL(-60_deg)
    SETBRK(COAST)
    wayfarer.traverseLinear({2.5_ft, 8.5_ft}, true);
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    
    CONVEYOR_ON
    wayfarer.traverseDistance(0.5_ft);
    TURNREL(0_deg)
    wayfarer.traverseDistance(3_ft);
    wayfarer.traverseDistance(-2_ft);

    arm_controller.setTarget(0_deg);
    TURNREL(-90_deg)
    wayfarer.traverseDistanceAsync(imu_odometer.getPosition().y);
    while(imu_odometer.getPosition().y > 3_ft)
    {
        DELAY(10_ms)
    }
    CONVEYOR_OFF
    CLAMP_CLOSE
    DELAY(250_ms)
    arm_controller.setTargetAsync(40_deg);
    wayfarer.waitUntilSettled();
    TURNREL(0_deg);
    wayfarer.traverseDistance(-imu_odometer.getPosition().x);
    CLIP_OPEN
    arm_controller.setTarget(0_deg);
    CLAMP_OPEN
}

void AutonomousRoutines::rightFullWP()
{   
    wayfarer.buildPath("Round_RSR", {3, 16_in, 30, 1})
        .withPoint({-1_ft, 0_ft})
        .withPoint({4.5_ft, 0_ft})
        .makeReversed()
        .generatePath();
    
    arm_controller.setTargetAsync(40_deg);
    
    SETBRK(COAST)
    wayfarer.asynchronousTraverse("Round_RSR");
    CLAMP_CLOSE
    wayfarer.waitUntilSettled();
    LOCK_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    arm_controller.setTargetAsync(0_deg);
    wayfarer.synchronousTraverse("Round_RSR_rev");

    CLAMP_OPEN
    DELAY(250_ms)
    wayfarer.traverseDistance(2_ft);
    CLAMP_CLOSE
    DELAY(250_ms)
    TURNRELSKILLS(-70_deg)
    DELAY(250_ms)
    arm_controller.setTarget(20_deg);

    SETBRK(COAST)
    wayfarer.traverseDistance(-1.6_ft);
    CLIP_CLOSE
    CONVEYOR_ON
    DELAY(250_ms)
    SETBRK(HOLD)

    arm_controller.setTarget(40_deg);
    arm_controller.setTarget(40_deg);
    wayfarer.traverseDistance(1.6_ft);
    TURNRELSKILLS(-90_deg)
    DELAY(250_ms)
    wayfarer.traverseDistance(5_ft);
    DELAY(3000_ms)
    CONVEYOR_OFF
}