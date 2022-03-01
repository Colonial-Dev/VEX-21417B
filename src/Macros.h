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
driveTrain->setMaxVelocity(std::clamp(x, 0, 200)); \
pathFinder->setMaxVelocity(std::clamp(x, 0, 200));

#define PATH(path) pathFinder->setTarget(path); pathFinder->waitUntilSettled();

#define PATHBACK(path) pathFinder->setTarget(path, true); pathFinder->waitUntilSettled();

#define PATHMIRROR(path) pathFinder->setTarget(path, false, true); pathFinder->waitUntilSettled();

#define PATHBACKMIRROR(path) pathFinder->setTarget(path, true, true); pathFinder->waitUntilSettled();

#define MAIN_LIFT_TARE \
frontLift->setTarget(0);\
frontLift->waitUntilSettled();

#define MAIN_LIFT_HOVER \
frontLift->setTarget(600);\
frontLift->waitUntilSettled();

#define MAIN_LIFT_BALANCE \
frontLift->setTarget(1200);\
frontLift->waitUntilSettled();

#define MAIN_LIFT_MAX \
frontLift->setTarget(2000);\
frontLift->waitUntilSettled();

#define MAIN_LIFT_CUSTOM(x) \
frontLift->setTarget(x);\
frontLift->waitUntilSettled();

#define FRONT_CLAMP_OPEN frontClamp->setTarget(360);
#define FRONT_CLAMP_CLOSE frontClamp->setTarget(-360);

#define PICKUP_FRONT FRONT_CLAMP_CLOSE MAIN_LIFT_HOVER
#define DROP_FRONT FRONT_CLAMP_OPEN MAIN_LIFT_TARE