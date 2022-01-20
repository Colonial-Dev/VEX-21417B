#pragma once

#define TURN(x) \
driveTrain->turnAngle((x/2) * degree);\
driveTrain->waitUntilSettled();

#define PATH(path) pathFinder->setTarget(path);

#define PATHBACK(path) pathFinder->setTarget(path, true);

#define WAIT pathFinder->waitUntilSettled();

//#define MAIN_LIFT_DOWN\ TBD

#define MAIN_LIFT_UP \
frontLiftRight->setTarget(600);\
frontLiftLeft->setTarget(-600);\
frontLiftLeft->waitUntilSettled();

#define REAR_LIFT_DOWN rearLift->setTarget(-5000);
#define REAR_LIFT_UP rearLift->setTarget(-700);
#define FRONT_CLAMP_OPEN frontClamp->setTarget(-360);
#define FRONT_CLAMP_CLOSE frontClamp->setTarget(360);

#define UNFOLD REAR_LIFT_DOWN FRONT_CLAMP_OPEN 