#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::skills()
{
    wayfarer.buildPath("Skills_1", {2, 18_in, 30, 1.0})
        .withOrigin()
        .withPoint({1.3_ft, -1.0_ft})
        .withPoint({5.5_ft, -0.1_ft})
        .withDebugDump()
        .generatePath();

    wayfarer.buildPath("Skills_2", {1.5, 16_in, 30, 0.6})
        .withPoint({5.5_ft, -0.1_ft})
        .withPoint({6_ft, 1_ft})
        .withPoint({9.65_ft, 2.25_ft})
        .withDebugDump()
        .generatePath();

    wayfarer.buildPath("Skills_3", {2, 18_in, 30, 0.5})
        .withPoint({8.5_ft, -2.25_ft})
        .withPoint({8.5_ft, 0.5_ft})
        .withPoint({4_ft, 4_ft})
        .generatePath();

    wayfarer.buildPath("Skills_4", {2, 16_in, 30, 0.5})
        .withPoint({4_ft, 4_ft})
        .withPoint({1.25_ft, 2.5_ft})
        .generatePath();

    wayfarer.buildPath("Skills_5", {2, 20_in, 30, 0.5})
        .withPoint({2.25_ft, 8_ft})
        .withPoint({6_ft, 7_ft})
        .generatePath();
    
    wayfarer.buildPath("Skills_6", {1.5, 16_in, 30, 0.5})
        .withPoint({6_ft, 6_ft})
        .withPoint({9_ft, 2.4_ft})
        .generatePath();

    lock_piston.set_value(true); //We don't want the lock engaging if it happens to be on
    SETROT(-90_deg) //We start facing left
    arm_controller.setTargetAsync(0_deg);

    CLAMP_UNFOLD

    CLIP_CLOSE //Clip mogo
    DELAY(350_ms)

    //Swerve out and coast into + grab small neutral
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Skills_1");
    CLAMP_CLOSE
    DELAY(250_ms)

    //Raise the arm to full height and path to the balance
    arm_controller.setTargetAsync(138_deg);
    CONVEYOR_ON
    wayfarer.synchronousTraverse("Skills_2");

    //Drop the small neutral on the balance
    CONVEYOR_OFF
    arm_controller.setTarget(70_deg);
    CLAMP_OPEN
    DELAY(250_ms)
    SETBRK(HOLD)

    //Back out and drop the mogo from the back clip
    arm_controller.setTargetAsync(100_deg);
    wayfarer.traverseDistance(-1_ft);
    TURNRELGOAL(90_deg);
    DELAY(250_ms)
    CLIP_OPEN
    DELAY(250_ms)
    wayfarer.traverseDistance(0.5_ft);

    //Lower the arm and turn to face the mogo
    arm_controller.setTargetAsync(0_deg);
    TURNREL(imu_odometer.getPosition().theta - 180_deg);

    //Coast into the mogo and grab it
    SETBRK(COAST)
    wayfarer.traverseDistance(1.5_ft);
    CLAMP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    arm_controller.setTargetAsync(110_deg);
    wayfarer.traverseDistance(-1.5_ft);

    //Slightly lift the mogo, turn to face the balance
    TURNRELGOAL(0_deg)
    wayfarer.traverseDistance(0.75_ft);
    
    arm_controller.setTarget(70_deg);
    CLAMP_OPEN
    DELAY(250_ms)

    //Back away from the balance and turn to face opposite the next alliance mogo
    arm_controller.setTargetAsync(100_deg);
    wayfarer.traverseDistance(-1.5_ft);
    arm_controller.setTargetAsync(0_deg);
    TURNREL(100_deg)

    //Start lowering the arm, and boost/coast into/clip the new mogo
    SETBRK(COAST)
    wayfarer.traverseLinear({8.25_ft, -2.1_ft}, true);
    CLIP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    //Path to the large neutral and grab it
    wayfarer.synchronousTraverse("Skills_3");
    CLAMP_CLOSE
    DELAY(250_ms)

    //Slightly raise the arm and correct to face the balance
    arm_controller.setTargetAsync(110_deg);
    DELAY(250_ms)
    
    //Raise the arm to stack height and path to the balance
    CONVEYOR_ON
    wayfarer.synchronousTraverse("Skills_4");

    //Drop large neutral - the long delays allow it to settle better
    arm_controller.setTarget(70_deg);
    arm_controller.setTarget(70_deg);
    DELAY(750_ms);
    CLAMP_OPEN
    DELAY(750_ms);

    //Back out and drop the mogo from the back clip
    wayfarer.traverseDistance(-0.5_ft);
    arm_controller.setTarget(100_deg);
    wayfarer.traverseDistance(-1.25_ft);
    CONVEYOR_OFF
    arm_controller.setTargetAsync(0_deg);
    DELAY(350_ms)
    CLIP_OPEN
    wayfarer.traverseDistance(0.5_ft);

    //Lower the arm and turn to face the mogo
    arm_controller.setTargetAsync(0_deg);
    TURNREL(imu_odometer.getPosition().theta + 180_deg);

    //Coast into the mogo and grab it
    SETBRK(COAST)
    wayfarer.traverseDistance(1.25_ft);
    CLAMP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    //Turn to face the balance and raise the arm to stacking height
    arm_controller.setTarget(20_deg);
    TURNRELGOAL(imu_odometer.getPosition().theta + 170_deg);
    arm_controller.setTarget(110_deg);

    //Drive to the balance and stack the mogo
    wayfarer.traverseDistance(3_ft);
    arm_controller.setTarget(70_deg);
    CLAMP_OPEN
    DELAY(250_ms)

    //CAREFULLY back away from the balance and turn to face opposite the next mogo
    wayfarer.traverseDistance(-0.5_ft);
    arm_controller.setTarget(120_deg);
    TURNREL(270_deg)
    arm_controller.setTargetAsync(0_deg);

    //Coast into + clip the new mogo
    SETBRK(COAST)
    wayfarer.traverseLinear({2.25_ft, 7.5_ft}, true);
    DELAY(250_ms)
    SETBRK(HOLD)
    CLIP_CLOSE
    DELAY(250_ms)

    //Boost/coast into/clamp the last small neutral
    SETBRK(COAST)
    wayfarer.synchronousTraverse("Skills_5");
    DELAY(250_ms)
    SETBRK(HOLD)
    CLAMP_CLOSE
    DELAY(250_ms)

    //Path to the far balance and stack the small neutral
    arm_controller.setTarget(30_deg);
    TURNRELGOAL(290_deg)
    arm_controller.setTarget(110_deg);
    wayfarer.synchronousTraverse("Skills_6");
    CLAMP_OPEN
    DELAY(250_ms)
    
    //Back away from the balance
    wayfarer.traverseDistance(-0.5_ft);
    arm_controller.setTarget(120_deg);
    arm_controller.setTargetAsync(0_deg);
    wayfarer.traverseDistance(-0.75_ft);

    //Turn to face right and unclip the mogo
    TURNREL(450_deg)
    SETROT(90_deg);
    DELAY(350_ms);
    CLIP_OPEN
    DELAY(250_ms)

    //Lower the arm, drive directly away from the mogo then turn to face it
    arm_controller.setTargetAsync(0_deg);
    wayfarer.traverseDistance(1.25_ft);
    TURNREL(-90_deg)

    //Coast into the mogo and grab it
    SETBRK(COAST)
    wayfarer.traverseDistance(1.4_ft);
    CLAMP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)
    
    //Slightly lift the mogo, turn to face the balance
    arm_controller.setTarget(30_deg);
    TURNRELGOAL(0_deg)
    DELAY(250_ms)

    //Raise the arm to stacking height and drive to + drop it on the balance
    arm_controller.setTarget(120_deg);
    wayfarer.traverseDistance(1.6_ft);
    arm_controller.setTarget(70_deg);
    CLAMP_OPEN
    DELAY(250_ms)

    //Back away from the balance
    /*wayfarer.traverseDistance(-0.5_ft);
    arm_controller.setTarget(120_deg);
    arm_controller.setTargetAsync(0_deg);
    wayfarer.traverseDistance(-1.5_ft);

    //Turn to face right, move to the end of the balance, then turn to face the last mogo
    TURNREL(90_deg)
    wayfarer.traverseLinear({9_ft, 6_ft});
    TURNREL(0_deg)

    //Very carefully grab last mogo and back away with it
    SETBRK(COAST)
    wayfarer.traverseDistance(2_ft);
    CLAMP_CLOSE
    DELAY(250_ms)
    SETBRK(HOLD)

    arm_controller.setTarget(20_deg);
    wayfarer.traverseDistance(-4_ft);
    
    //Turn to face the near balance, lift the mogo to stack height and GO GO GO
    TURNREL(190_deg)
    arm_controller.setTargetAsync(120_deg);

    wayfarer.buildPath("Skills_Final", {1.5, 16_in, 30, 0.5})
        .withCurrentPosition(imu_odometer)
        .withPoint({1.25_ft, 2.25_ft})
        .generatePath();

    wayfarer.synchronousTraverse("Skills_Final");
    arm_controller.setTarget(70_deg);
    CLAMP_OPEN
    pros::delay(250);*/
}