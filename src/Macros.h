#pragma once

#define EMERGENCY_STOP abort();

#define PATHGEN(x, y, deg, name) \
pathFinder->generatePath({ \
    {0_ft, 0_ft, 0_deg}, \
    {x ## _ft, y ## _ft, deg ## _deg}}, \
    name \
);

#define DELAY(x) \
pros::delay(x);

#define TURN(x) \
driveTrain->turnAngle((x/2) * degree);\
driveTrain->waitUntilSettled();

#define THROTTLE(x) \
driveTrain->setMaxVelocity(std::clamp(x, 0, 600)); \
pathFinder->setMaxVelocity(std::clamp(x, 0, 600));

#define WAIT pathFinder->waitUntilSettled();

#define PATH(path) pathFinder->setTarget(path); WAIT

#define PATHBACK(path) pathFinder->setTarget(path, true); WAIT

#define MAIN_LIFT_HOVER \
frontLiftRight->setTarget(600);\
frontLiftLeft->setTarget(-600);\
frontLiftLeft->waitUntilSettled();

#define MAIN_LIFT_TARE \
frontLiftRight->setTarget(0);\
frontLiftLeft->setTarget(0);\
frontLiftLeft->waitUntilSettled();

#define REAR_LIFT_DOWN rearLift->setTarget(0);
#define REAR_LIFT_UP rearLift->setTarget(4300); DELAY(1000);
#define FRONT_CLAMP_OPEN frontClamp->setTarget(-360);
#define FRONT_CLAMP_CLOSE frontClamp->setTarget(360);

#define PICKUP_FRONT FRONT_CLAMP_CLOSE MAIN_LIFT_HOVER
#define DROP_FRONT FRONT_CLAMP_OPEN MAIN_LIFT_TARE