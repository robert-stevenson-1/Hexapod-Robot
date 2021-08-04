#ifndef Robot_Structure_h
#define Robot_Structure_h

#include <Servo.h>

typedef struct{
  int femur;
  int tibia;

  Servo rotate;
  int rotateAngle;

  Servo lift;
  int liftAngle;

  Servo knee;
  int kneeAngle;
} Leg;

#endif
