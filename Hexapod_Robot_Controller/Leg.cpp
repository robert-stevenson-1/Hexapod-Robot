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

    Leg::rotationAngle = hipRotate.read();
    Leg::liftAngle = hipLift.read();
    Leg::kneeAngle = knee.read();

    Leg::isLeft = isLeft;
}

void Leg::moveLeg(int rotateAngle, int liftAngle, int kneeAngle) {
  if(Leg::isLeft){
    hipLift.write(180-liftAngle);
    delay(SERVO_WRITE_DELAY);
    
    hipRotate.write(180-rotateAngle);
    delay(SERVO_WRITE_DELAY);

    knee.write(180-kneeAngle);
    delay(SERVO_WRITE_DELAY);
  
  } else {
    hipLift.write(liftAngle);
    delay(SERVO_WRITE_DELAY);
    
    hipRotate.write(rotateAngle);
    delay(SERVO_WRITE_DELAY);
    
    knee.write(kneeAngle);
    delay(SERVO_WRITE_DELAY);
  }
    Leg::liftAngle = hipLift.read();
    delay(SERVO_WRITE_DELAY);
    Leg::rotationAngle = hipRotate.read();
    delay(SERVO_WRITE_DELAY);
    Leg::kneeAngle = knee.read();
    delay(SERVO_WRITE_DELAY);
}

int Leg::getRotationAngle() {
  if(Leg::isLeft){
    return 180-rotationAngle;
  }else{
    return rotationAngle;
  }
}

int Leg::getLiftAngle() {
  if(Leg::isLeft){
    return 180-liftAngle;
  }else{
    return liftAngle;
  }
}

int Leg::getKneeAngle() {
  if(Leg::isLeft){
    return 180-kneeAngle;
  }else{
    return kneeAngle;
  }
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
