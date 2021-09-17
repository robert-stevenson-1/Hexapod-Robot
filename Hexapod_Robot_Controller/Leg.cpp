//
// Created by Rls20 on 17/09/2021.
//

#include "Leg.h"
#include "RobotConfig.h"
#include <Arduino.h>

Leg::Leg() {
    Leg::rotationAngle = hipRotate.read();
    Leg::liftAngle = hipLift.read();
    Leg::kneeAngle = knee.read();
}

Leg::Leg(int hipRotateServoPin, int hipLiftServoPin, int kneeServoPin, bool isLeft) {
    hipRotate.attach(hipRotateServoPin);
    hipLift.attach(hipLiftServoPin);
    knee.attach(kneeServoPin);

//    Leg::rotationAngle = hipRotate.read();
  //  Leg::liftAngle = hipLift.read();
    //Leg::kneeAngle = knee.read();

    Leg::isLeft = isLeft;
}

void Leg::moveLeg(int rotateAngle, int liftAngle, int kneeAngle) {
    hipRotate.write(rotateAngle);
    delay(SERVO_WRITE_DELAY);
    hipLift.write(liftAngle);
    delay(SERVO_WRITE_DELAY);
    knee.write(kneeAngle);
    delay(SERVO_WRITE_DELAY);
    
//    Leg::rotationAngle = hipRotate.read();
  //  Leg::liftAngle = hipLift.read();
    //Leg::kneeAngle = knee.read();
}

int Leg::getRotationAngle() {
    return rotationAngle;
}

int Leg::getLiftAngle() {
    return liftAngle;
}

int Leg::getKneeAngle() {
    return kneeAngle;
}


Servo &Leg::getHipRotate() {
    return hipRotate;
}

Servo &Leg::getHipLift() {
    return hipLift;
}

Servo &Leg::getKnee() {
    return knee;
}
