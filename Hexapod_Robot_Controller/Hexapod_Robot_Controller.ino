#include <Servo.h>

struct leg{
  Servo rotate;
  int femur;
  Servo lift;
  int tibia;
  Servo knee;
  
  int rotateInitAngle;
  int liftInitAngle;
  int kneeInitAngle;
};

leg legs[6];

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
