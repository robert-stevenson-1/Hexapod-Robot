#ifndef Robot_Structure_h
#define Robot_Structure_h

#include <Servo.h>

struct RobotData{
    short FrRotationAngle;
    short FrLiftAngle;
    short FrKneeAngle;

    short MrRotationAngle;
    short MrLiftAngle;
    short MrKneeAngle;

    short BrRotationAngle;
    short BrLiftAngle;
    short BrKneeAngle;

    short FlRotationAngle;
    short FlLiftAngle;
    short FlKneeAngle;

    short MlRotationAngle;
    short MlLiftAngle;
    short MlKneeAngle;

    short BlRotationAngle;
    short BlLiftAngle;
    short BlKneeAngle;
};

#endif
