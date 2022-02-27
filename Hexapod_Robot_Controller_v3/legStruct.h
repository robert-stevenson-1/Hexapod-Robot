#ifndef legStruct_h
#define legStruct_h

#include <Servo.h>
struct leg{
  //Leg Stucture
  Servo hipRotate;
  int curHip;
  Servo hipLift;
  int curLift;
  Servo knee;
  int curKnee;
  bool isLeft;

  //Var to flag if leg need moving
  bool inPos = false;
  
  //IK
  float feetPos_X;
  float feetPos_Y;
  float feetPos_Z;
  
  float bodyOffsetX;
  float bodyOffsetZ;
};
#endif
