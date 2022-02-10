#pragma once

#define DELAY(x) \
pros::delay(x);

#define TURN(x) \
driveTrain->turnAngle((x/2) * degree);\
driveTrain->waitUntilSettled();

#define THROTTLE(x) \
driveTrain->setMaxVelocity(std::clamp(x, 0, 600)); \
pathFinder->setMaxVelocity(std::clamp(x, 0, 600));

#define PATH(path) pathFinder->setTarget(path);

#define PATHBACK(path) pathFinder->setTarget(path, true);

#define WAIT pathFinder->waitUntilSettled();

#define MAIN_LIFT_HOVER \
frontLiftRight->setTarget(400);\
frontLiftLeft->setTarget(-400);\
frontLiftLeft->waitUntilSettled();

#define MAIN_LIFT_TARE \
frontLiftRight->setTarget(0);\
frontLiftLeft->setTarget(0);\
frontLiftLeft->waitUntilSettled();

#define REAR_LIFT_DOWN rearLift->setTarget(0);
#define REAR_LIFT_UP rearLift->setTarget(4300);
#define FRONT_CLAMP_OPEN frontClamp->setTarget(-360);
#define FRONT_CLAMP_CLOSE frontClamp->setTarget(360);