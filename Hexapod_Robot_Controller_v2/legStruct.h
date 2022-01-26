#ifndef legStruct_h
#define legStruct_h

#include <Servo.h>
struct leg{
    Servo hipRotate;
    Servo hipLift;
    Servo knee;
    bool isLeft;
    float feetPos_X;
    float feetPos_Y;
    float feetPos_Z;
};
#endif
