#include <MPU6050.h>
#include <HCSR04.h>
#include <Servo.h>
#include "Robot.h"

// ======================
// ========STRUCT========
// ======================

// =======================
// =======VARIABLES=======
// =======================

//ultrasonic distance sensor
double distance = 0;

//Gyro and Accelerometer data
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Robot head Servos
Servo headRotate;
Servo headLift;

Robot robot;
//Leg fr;

void setup() {
  //serial monitor
  Serial.begin(9600);

  robot = Robot();
  
  //Setup the ultrasonic sensor
  HCSR04.begin(TRIG_PIN, ECHO_PIN);

  //setup the MPU6050
  Wire.begin();
  mpu.initialize();

  // === ATTACH SERVOS ==
  attachServos();
  delay(100);
  // ====================

  // === SET LEGS TO INITIAL ANGLES ===

  //fr.moveLeg(0, 0 , 0);
  robot.updateLeg(&robot.getFr(), FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  delay(20);
  robot.updateLeg(&robot.getMr(), MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  delay(20);
  robot.updateLeg(&robot.getBr(), BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  delay(20);
  robot.updateLeg(&robot.getFl(), FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  delay(20);
  robot.updateLeg(&robot.getMl(), ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  delay(20);
  robot.updateLeg(&robot.getBl(), BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);

  // ==================================



  Serial.println("ax ay "); //az
  Serial.println("gx gy gz");
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = *(HCSR04.measureDistanceCm());

  Serial.println(robot.getBl().getHipRotate().attached());
  //get balance data
  //getBalanceData();
  // mpu print data
  /*
    Serial.print(mpu.getAccelerationX());
    Serial.print(" ");
    Serial.println(mpu.getAccelerationY());  
    Serial.print(" ");
    Serial.print(mpu.getRotationX());
    Serial.print(" ");
    Serial.print(mpu.getRotationY());
    Serial.print(" ");
    Serial.print(mpu.getRotationZ());
    Serial.print(" ");
        */
  //set the new leg angles
  
  
  //send data

  delay(100);
}

void serialMonitorTest(){
  String angleStr = "";
  int angle = 90;
  if(Serial.available() > 0){
    angleStr = Serial.readString();
    angle = angleStr.toInt();
    Serial.print("Current Angle: ");
    Serial.print(angle);
    //fr.moveLeg(angle, 90 , 90);
    Serial.print("| Hip (Rotate) Angle: ");
    //Serial.print(fr.getHipRotate().read());
    Serial.print("| Hip (Lift) Angle: ");
    //Serial.print(fr.getHipLift().read());
    Serial.print("| Knee Angle: ");
    //Serial.println(fr.getKnee().read());
    angle = 0;
  }
}

void attachServos() {
  robot.getFr() = Leg(FR_ROTATE_PIN, FR_LIFT_PIN, FR_KNEE_PIN, false);
  delay(10);
  robot.getMr() = Leg(MR_ROTATE_PIN, MR_LIFT_PIN, MR_KNEE_PIN, false);
  delay(10);
  robot.getBr() = Leg(BR_ROTATE_PIN, BR_LIFT_PIN, BR_KNEE_PIN, false);
  delay(10);
  robot.getFl() = Leg(FL_ROTATE_PIN, FL_LIFT_PIN, FL_KNEE_PIN, true);
  delay(10);
  robot.getMl() = Leg(ML_ROTATE_PIN, ML_LIFT_PIN, ML_KNEE_PIN, true);
  delay(10);
  robot.getBl() = Leg(BL_ROTATE_PIN, BL_LIFT_PIN, BL_KNEE_PIN, true);
  delay(10);
}
/*
void attachServos() {
  // ===> LEGS:
  //Setup the Front right legs
  robot.getFr().getHipRotate().attach(FR_ROTATE_PIN);
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

void legSetAngles(Leg *leg, int _rotateAngle, int _liftAngle, int _kneeAngle) {
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

void updateLeg(Leg *leg, Leg *newLeg) {
  leg->rotate.write(newLeg->rotateAngle);
  leg->rotateAngle = newLeg->rotateAngle;
  delay(SERVO_WRITE_DELAY);

  leg->lift.write(newLeg->liftAngle);
  leg->liftAngle = newLeg->liftAngle;
  delay(SERVO_WRITE_DELAY);

  leg->knee.write(newLeg->kneeAngle);
  leg->kneeAngle = newLeg->kneeAngle;
  delay(SERVO_WRITE_DELAY);
}
*/
void getBalanceData() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //ax = map(ax, -17000, 17000, 0, 255 ); // X axis data
  //ay = map(ay, -17000, 17000, 0, 255); // Y axis data
  //az = map(az, -17000, 17000, 0, 255);  // Z axis data
}
