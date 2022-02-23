#pragma once

#define EMERGENCY_STOP abort();

#define PATHGEN(x, y, deg, name) \
pathFinder->generatePath({ \
    {0_ft, 0_ft, 0_deg}, \
    {x ## _ft, y ## _ft, deg ## _deg}}, \
    name \
); computedPaths++;

#define DELAY(x) \
pros::delay(x);

#define TURN(x) \
driveTrain->turnAngle((x*7.68) * degree);\
driveTrain->waitUntilSettled();

#define THROTTLE(x) \
driveTrain->setMaxVelocity(std::clamp(x, 0, 600)); \
pathFinder->setMaxVelocity(std::clamp(x, 0, 600));

#define WAIT pathFinder->waitUntilSettled();

#define PATH(path) pathFinder->setTarget(path); pathFinder->waitUntilSettled();

#define PATHBACK(path) pathFinder->setTarget(path, true); pathFinder->waitUntilSettled();

#define PATHMIRROR(path) pathFinder->setTarget(path, false, true); pathFinder->waitUntilSettled();

#define PATHBACKMIRROR(path) pathFinder->setTarget(path, true, true); pathFinder->waitUntilSettled();

#define MAIN_LIFT_HOVER \
frontLiftRight->setTarget(600);\
frontLiftLeft->setTarget(-600);\
frontLiftLeft->waitUntilSettled();

#define MAIN_LIFT_TARE \
frontLiftRight->setTarget(0);\
frontLiftLeft->setTarget(0);\
frontLiftLeft->waitUntilSettled();

#define REAR_LIFT_DOWN rearLift->setTarget(-2150);
#define REAR_LIFT_UP rearLift->setTarget(2150); DELAY(1000);
#define REAR_LIFT_TARE rearLift->setTarget(0);

#define FRONT_CLAMP_OPEN frontClamp->setTarget(360);
#define FRONT_CLAMP_CLOSE frontClamp->setTarget(-360);

#define PICKUP_FRONT FRONT_CLAMP_CLOSE MAIN_LIFT_HOVER
#define DROP_FRONT FRONT_CLAMP_OPEN MAIN_LIFT_TARE