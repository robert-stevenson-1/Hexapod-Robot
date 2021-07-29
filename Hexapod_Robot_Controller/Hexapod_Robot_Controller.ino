#include <MPU6050.h>

#include <HCSR04.h>
#include <Servo.h>

// =======================
// =========CONFIG========
// =======================

// == SERVO WAIT DELAY ==
#define SERVO_WRITE_DELAY 15
// ======================

// === LEG SERVO INTIAL ANGLES ===
#define FR_ROTATE_INIT_ANGLE 0
#define FR_LIFT_INIT_ANGLE 0
#define FR_KNEE_INIT_ANGLE 0

#define MR_ROTATE_INIT_ANGLE 0
#define MR_LIFT_INIT_ANGLE 0
#define MR_KNEE_INIT_ANGLE 0

#define BR_ROTATE_INIT_ANGLE 0
#define BR_LIFT_INIT_ANGLE 0
#define BR_KNEE_INIT_ANGLE 0

#define FL_ROTATE_INIT_ANGLE 0
#define FL_LIFT_INIT_ANGLE 0
#define FL_KNEE_INIT_ANGLE 0

#define ML_ROTATE_INIT_ANGLE 0
#define ML_LIFT_INIT_ANGLE 0
#define ML_KNEE_INIT_ANGLE 0

#define BL_ROTATE_INIT_ANGLE 0
#define BL_LIFT_INIT_ANGLE 0
#define BL_KNEE_INIT_ANGLE 0

// ===============================

// === LEG SERVO PINS ===
#define FR_ROTATE_PIN 22
#define FR_LIFT_PIN 23
#define FR_KNEE_PIN 24

#define MR_ROTATE_PIN 25
#define MR_LIFT_PIN 26
#define MR_KNEE_PIN 27

#define BR_ROTATE_PIN 28
#define BR_LIFT_PIN 29
#define BR_KNEE_PIN 30

#define FL_ROTATE_PIN 31
#define FL_LIFT_PIN 32
#define FL_KNEE_PIN 33

#define ML_ROTATE_PIN 34
#define ML_LIFT_PIN 35
#define ML_KNEE_PIN 36

#define BL_ROTATE_PIN 37
#define BL_LIFT_PIN 38
#define BL_KNEE_PIN 39

// ======================

// === HEAD LEG PINS ===

#define HEAD_ROTATE_PIN 40
#define HEAD_LIFT_PIN 41

// =====================

// === ULTRASONIC PINS ===

#define ECHO_PIN 42
#define TRIG_PIN 43

// =======================

// ======================
// ========STRUCT========
// ======================

struct leg{
  int femur;
  int tibia;
  
  Servo rotate;
  int rotateAngle;

  Servo lift;
  int liftAngle;

  Servo knee;
  int kneeAngle;
};

// =======================
// =======VARIABLES=======
// =======================

//ultrasonic distance sensor
int distance = 0;

//Gyro and Accelerometer data 

//Robot head Servos
Servo headRotate;
Servo headLift;

//Robot legs
leg frontRight;
leg middleRight;
leg backRight;

leg frontLeft;
leg middleLeft;
leg backLeft;



void setup() {
  //Setup the ultrasonic sensor
  HCSR04.begin(TRIG_PIN, ECHO_PIN);

  // === ATTACH SERVOS ==
  attachServos();
  // ====================
  
  // === SET LEGS TO INITIAL ANGLES ===

  legSetAngles(&frontRight, FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  legSetAngles(&middleRight, MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  legSetAngles(&backRight, BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);

  legSetAngles(&frontLeft, FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  legSetAngles(&middleLeft, ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  legSetAngles(&backLeft, BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);
  
  // ==================================


  
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = HCSR04.measureDistanceCm();

  //get balance data
  
  //get new angles from input data (from ROS)

  //set the new leg angles

  //send data
}

void attachServos(){
  // ===> LEGS:
  //Setup the Front right legs
  frontRight.rotate.attach(FR_ROTATE_PIN);
  frontRight.lift.attach(FR_LIFT_PIN);
  frontRight.knee.attach(FR_KNEE_PIN);

  //Setup the Middle right legs
  middleRight.rotate.attach(MR_ROTATE_PIN);
  middleRight.lift.attach(MR_LIFT_PIN);
  middleRight.knee.attach(MR_KNEE_PIN);
  
  //Setup the Bsck right legs
  backRight.rotate.attach(BR_ROTATE_PIN);
  backRight.lift.attach(BR_LIFT_PIN);
  backRight.knee.attach(BR_KNEE_PIN);

  //Setup the Front left legs
  frontLeft.rotate.attach(FL_ROTATE_PIN);
  frontLeft.lift.attach(FL_LIFT_PIN);
  frontLeft.knee.attach(FL_KNEE_PIN);
  
  
  //Setup the Middle left legs
  middleLeft.rotate.attach(ML_ROTATE_PIN);
  middleLeft.lift.attach(ML_LIFT_PIN);
  middleLeft.knee.attach(ML_KNEE_PIN);

  //Setup the Back left legs
  backLeft.rotate.attach(BL_ROTATE_PIN);
  backLeft.lift.attach(BL_LIFT_PIN);
  backLeft.knee.attach(BL_KNEE_PIN);

  // ===> HEAD:
  headRotate.attach(HEAD_ROTATE_PIN);
  headLift.attach(HEAD_LIFT_PIN);  


}

void legSetAngles(leg *leg, int _rotateAngle, int _liftAngle, int _kneeAngle){
  leg->rotate.write(_rotateAngle);
  leg->rotateAngle = _rotateAngle;
  delay(SERVO_WRITE_DELAY);
  
  leg->lift.write(_liftAngle);
  leg->liftAngle = _liftAngle;
  delay(SERVO_WRITE_DELAY);
  
  leg->knee.write(_kneeAngle);
  leg->kneeAngle = _kneeAngle;
  delay(SERVO_WRITE_DELAY);
  
}
