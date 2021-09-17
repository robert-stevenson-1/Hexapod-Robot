//
// Created by Rls20 on 17/09/2021.
//

#ifndef ROBOT___C___LEG_H
#define ROBOT___C___LEG_H


#include "Servo.h"

class Leg {

private:
    Servo hipRotate;
    Servo hipLift;
    Servo knee;

    int rotationAngle;
    int liftAngle;
    int kneeAngle;

    bool isLeft;
public:
    Leg();
    Leg(int hipRotateServoPin, int hipLiftServoPin, int kneeServoPin, bool isLeft);
    void moveLeg(int rotateAngle, int liftAngle, int kneeAngle);
    int getRotationAngle();
    int getLiftAngle();
    int getKneeAngle();
    Servo &getHipRotate();
    Servo &getHipLift();
    Servo &getKnee();
};


#endif //ROBOT___C___LEG_H
