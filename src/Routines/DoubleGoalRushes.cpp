#include "robokauz/ROUTINES.hpp"

void AutonomousRoutines::leftDoubleRush()
{
    AutonomousRoutines::leftSingleRush();
    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_LDR", DEFAULT_GEN_PARAMS)
        .withCurrentPosition(imu_odometer)
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
    //Drop small neutral, zip out and grab large neutral, retreat
    wayfarer.buildPath("Round_RDR", DEFAULT_GEN_PARAMS)
        .withCurrentPosition(imu_odometer)
        .withPoint({4.5_ft, -4_ft})
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