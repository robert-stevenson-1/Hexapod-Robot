#include <MPU6050.h>
#include <HCSR04.h>
#include <Servo.h>
#include "Robot.h"
#include "RobotData.h"

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

//Robot Structure and data
Robot robot;
//Leg fr;
String data = "";

void setup() {
  //serial monitor
  Serial.begin(9600);
  Serial.flush();
  //robot = Robot();
  //fr = Leg(FL_ROTATE_PIN, FL_LIFT_PIN, FL_KNEE_PIN, true);
  
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
/*
  //fr.moveLeg(90, 90, 90);
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
*/
  // ==================================

  Serial.println("ax ay "); //az
  Serial.println("gx gy gz");

  data = "";
}

void loop() {
  #ifdef NORMAL
  
  //send data
  if(data != ""){
    Serial.flush();
    Serial.println(data);
    data = "";
    robot.updateLeg(&robot.getBr(), BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  }

  delay(20);
  
  //Read Data in
  if(Serial.available() > 0){
    Serial.flush();
    data = "";
    data = Serial.readString();
    robot.updateLeg(&robot.getBr(), BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE-50);
  }
  // put your main code here, to run repeatedly:
  //distance = *(HCSR04.measureDistanceCm());

  //Serial.println(robot.getBl().getHipRotate().attached());
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
  //--rotate on the spot--

  //rotate(25, 30, 100);
  //delay(1000);
  
  delay(100);
  #endif
}

void rotate(int rotVal, int liftVal, int delVal){
    
  //==First 3
  //Move FL up
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle(), robot.getFl().getLiftAngle() + liftVal, robot.getFl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle(), robot.getFl().getLiftAngle(), robot.getFl().getKneeAngle() + liftVal);
  delay(delVal);

  //Move BL up
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle(), robot.getBl().getLiftAngle() + liftVal, robot.getBl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle(), robot.getBl().getLiftAngle(), robot.getBl().getKneeAngle() + liftVal);
  delay(delVal);

  //Move MR up
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle(), robot.getMr().getLiftAngle() + liftVal, robot.getMr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle(), robot.getMr().getLiftAngle(), robot.getMr().getKneeAngle() + liftVal);
  delay(delVal);

  //FL to the right and down
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle() - rotVal, robot.getFl().getLiftAngle(), robot.getFl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle(), robot.getFl().getLiftAngle(), robot.getFl().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle(), robot.getFl().getLiftAngle() - liftVal, robot.getFl().getKneeAngle());
  delay(delVal);

  //BL to the right and down
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle() - rotVal, robot.getBl().getLiftAngle(), robot.getBl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle(), robot.getBl().getLiftAngle(), robot.getBl().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle(), robot.getBl().getLiftAngle() - liftVal, robot.getBl().getKneeAngle());
  delay(delVal);

  //MR to the right and down
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle() - rotVal, robot.getMr().getLiftAngle(), robot.getMr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle(), robot.getMr().getLiftAngle(), robot.getMr().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle(), robot.getMr().getLiftAngle() - liftVal, robot.getMr().getKneeAngle());
  delay(delVal);

  //==second 3

  //Move FR up
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle(), robot.getFr().getLiftAngle() + liftVal, robot.getFr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle(), robot.getFr().getLiftAngle(), robot.getFr().getKneeAngle() + liftVal);
  delay(delVal);

  //Move BR up
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle(), robot.getBr().getLiftAngle() + liftVal, robot.getBr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle(), robot.getBr().getLiftAngle(), robot.getBr().getKneeAngle() + liftVal);
  delay(delVal);

  //Move ML up
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle(), robot.getMl().getLiftAngle() + liftVal, robot.getMl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle(), robot.getMl().getLiftAngle(), robot.getMl().getKneeAngle() + liftVal);
  delay(delVal);

  //FR to the right and down
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle() - rotVal, robot.getFr().getLiftAngle(), robot.getFr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle(), robot.getFr().getLiftAngle(), robot.getFr().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle(), robot.getFr().getLiftAngle() - liftVal, robot.getFr().getKneeAngle());
  delay(delVal);

  //BR to the right and down
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle() - rotVal, robot.getBr().getLiftAngle(), robot.getBr().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle(), robot.getBr().getLiftAngle(), robot.getBr().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle(), robot.getBr().getLiftAngle() - liftVal, robot.getBr().getKneeAngle());
  delay(delVal);

  //ML to the right and down
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle() - rotVal, robot.getMl().getLiftAngle(), robot.getMl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle(), robot.getMl().getLiftAngle(), robot.getMl().getKneeAngle() - liftVal);
  delay(delVal);
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle(), robot.getMl().getLiftAngle() - liftVal, robot.getMl().getKneeAngle());
  delay(delVal);

  //==Center the hip servos
  robot.updateLeg(&robot.getFl(), robot.getFl().getRotationAngle()+ rotVal, robot.getFl().getLiftAngle(), robot.getFl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getFr(), robot.getFr().getRotationAngle()+ rotVal, robot.getFr().getLiftAngle(), robot.getFr().getKneeAngle());
  delay(delVal);
  
  robot.updateLeg(&robot.getMl(), robot.getMl().getRotationAngle() + rotVal, robot.getMl().getLiftAngle(), robot.getMl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getMr(), robot.getMr().getRotationAngle() + rotVal, robot.getMr().getLiftAngle(), robot.getMr().getKneeAngle());
  delay(delVal);
    
  robot.updateLeg(&robot.getBl(), robot.getBl().getRotationAngle()+ rotVal, robot.getBl().getLiftAngle(), robot.getBl().getKneeAngle());
  delay(delVal);
  robot.updateLeg(&robot.getBr(), robot.getBr().getRotationAngle()+ rotVal, robot.getBr().getLiftAngle(), robot.getBr().getKneeAngle());
  delay(delVal); 
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


void serialMonitorLegControl(){
  String cmd[3] = {"","",""};
  int angle = 90;
  if(Serial.available() > 0){
    cmd[0] = Serial.readStringUntil(' ');
    cmd[0] = Serial.readStringUntil(' ');
    cmd[0] = Serial.readStringUntil('\n');
    angle = cmd[2].toInt();
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
