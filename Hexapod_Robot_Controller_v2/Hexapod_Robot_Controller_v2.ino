#include <MPU6050.h>
#include <HCSR04.h>
#include <math.h>
//#include <Servo.h>
//#include "Robot.h"
#include "LegStruct.h"
#include "RobotConfig.h"

// ======================
// =========ENUMS========
// ======================

enum legVal{
  FR = 1,
  MR = 2,
  BR = 3,
  FL = 4,
  ML = 5,
  BL = 6
};

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
//Robot robot;
leg fr;
leg fl;
leg mr;
leg ml;
leg br;
leg bl;

//Comm Data
const byte maxData = 128;
char data[maxData];
char outboundData[maxData];
bool newData = false;

//test vars
int a = 0;
bool up = true;
void setup() {
  //serial monitor
  Serial.begin(9600);
  Serial.flush();
  Serial.setTimeout(1000);

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

  #ifdef NORMAL
  //fr.moveLeg(90, 90, 90);
  moveLeg(&fr, FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&mr, MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&br, BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&fl, FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&ml, ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&bl, BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);
  #endif
  
  #ifdef MAINTENANCE
  moveLeg(&fr, FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&mr, MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&br, BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&fl, FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&ml, ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  delay(20);
  moveLeg(&bl, BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);
  #endif

  
  #ifdef IK_MODE
  //fr.moveLeg(90, 90, 90);
  //moveLegIK(&fr, 0, 0, 0);
  delay(20);
  //moveLegIK(&mr, 0, 0, 0);
  delay(20);
  moveLegIK(&br, 0, 0, 0);
  delay(20);
  //moveLegIK(&fl, 0, 0, 0);
  delay(20);
  //moveLegIK(&ml, 0, 0, 0);
  delay(20);
  //moveLegIK(&bl, 0, 0, 0);
  #endif
  // ==================================

  Serial.println("ax ay "); //az
  Serial.println("gx gy gz");
  clearInputBuffer();
}

void loop() {
  #ifdef IK_MODE//NORMAL
  //Serial.flush();
  
  //Read Data in
  getData();
  
  //after getting new data set the angles after parsing
  if(newData == true){

    Serial.println(data);
    
    //parse data to get angles
    char *comData[4];
    // Example of Command: <1 90 90 90> (leg: 1(FR) | Hip: 90deg | lift: 90deg | Knee: 90deg)
    getComData(data, comData, 4, " ");
    for(int i = 0; i < 4; i++){
      Serial.print(" | ");
      Serial.print(comData[i]);
    }

    Serial.print("Leg: ");
    Serial.println(comData[0]);
    switch(atoi(comData[0])){
      case FR: // 1
        Serial.println("FR");
        moveLegIK(&fr, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
      case MR: // 2
        Serial.println("MR");
        moveLegIK(&mr, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
      case BR: // 3
        Serial.println("BR");
        moveLegIK(&br, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
      case FL: // 4
        Serial.println("FL");
        moveLegIK(&fl, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
      case ML: // 5
        Serial.println("ML");
        moveLegIK(&ml, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
      case BL: // 6 
        Serial.println("BL");
        moveLegIK(&bl, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
    }


//    moveLeg(&fr, angles[0], angles[1], angles[2]);
//    delay(20);
//    moveLeg(&mr, angles[3], angles[4], angles[5]);
//    delay(20);
//    moveLeg(&br, angles[6], angles[7], angles[8]);
//    delay(20);
//    moveLeg(&fl, angles[9], angles[10], angles[11]);
//    delay(20);
//    moveLeg(&ml, angles[12], angles[13], angles[14]);
//    delay(20);
//    moveLeg(&bl, angles[15], angles[16], angles[17]);

  delay(4000);
  
  }else{

//     if(up){
//      if(a >= 50){
//        up = false;
//      }
////      moveLegIK(&fr, 0, a, 0);
////      moveLegIK(&fl, 0, a, 0);
////      moveLegIK(&mr, 0, a, 0);
////      moveLegIK(&ml, 0, a, 0);
////      moveLegIK(&br, 0, a, 0);
//      moveLegIK(&bl, 0, a, 0);
//      a += 10;
//     }else{
//      if(a <= -30){
//        up = true;
//      }
////      moveLegIK(&fr, 0, a, 0);
////      moveLegIK(&fl, 0, a, 0);
////      moveLegIK(&mr, 0, a, 0);
////      moveLegIK(&ml, 0, a, 0);
////      moveLegIK(&br, 0, a, 0);
//      moveLegIK(&bl, 0, a, 0);
//      a -= 10;
//     }
//     delay (1000);
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

  //Reset fetched data
  if(newData == true){
    newData = false;
    memset(data, 0, sizeof(data)); //clear the arrayz
    clearInputBuffer();
  }
  #endif 
}

void getData() {
    static boolean readingInProgress = false;
    static byte ndx = 0;
    char startCharMark = '<';
    char endCharMark = '>';
    char readChar;

    while (Serial.available() > 0 && newData == false) {
        readChar = Serial.read();

        if (readingInProgress == true) {
            if (readChar != endCharMark) {
                data[ndx] = readChar;
                ndx++;
                if (ndx >= maxData) {
                    ndx = maxData - 1;
                }
            }
            else {
                data[ndx] = '\0'; // terminate the string
                readingInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (readChar == startCharMark) {
            readingInProgress = true;
        }
    }
}

void clearInputBuffer(){
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void getComData(char* data, char** retData, const int arrSize,const char* delim){

  //Serial.println("Started Parsing");
  int i = 0;
  
  char* d = strtok(data, delim);
  while (d != NULL) {
    retData[i] = d;
    //Serial.println (retData[i]);
    d = strtok(NULL, delim);
    i++;
  }

  //Serial.println("Done Parsing");
}

/*
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
*/
void attachServos() {
  //Right Side
  
  //Setup the the Front Right leg
  fr.hipRotate.attach(FR_ROTATE_PIN);
  fr.hipLift.attach(FR_LIFT_PIN);
  fr.knee.attach(FR_KNEE_PIN);
  fr.isLeft = false;
  fr.feetPos_X = FR_X;
  fr.feetPos_Y = FR_Y;
  fr.feetPos_Z = FR_Z;
  fr.bodyOffsetX = BODY_CENTER_OFFSET_1;
  fr.bodyOffsetY = BODY_CENTER_OFFSET_2;
  //delay(20);
  //Setup the the Middle Right leg
  mr.hipRotate.attach(MR_ROTATE_PIN);
  mr.hipLift.attach(MR_LIFT_PIN);
  mr.knee.attach(MR_KNEE_PIN);
  mr.isLeft = false;
  mr.feetPos_X = MR_X;
  mr.feetPos_Y = MR_Y;
  mr.feetPos_Z = MR_Z;
  mr.bodyOffsetX = BODY_SIDE_LENGTH;
  mr.bodyOffsetY = 0;
  //delay(20);
  //Setup the the Back Right leg
  br.hipRotate.attach(BR_ROTATE_PIN);
  br.hipLift.attach(BR_LIFT_PIN);
  br.knee.attach(BR_KNEE_PIN);
  br.isLeft = false;
  br.feetPos_X = BR_X;
  br.feetPos_Y = BR_Y;
  br.feetPos_Z = BR_Z;
  br.bodyOffsetX = BODY_CENTER_OFFSET_1;
  br.bodyOffsetY = -BODY_CENTER_OFFSET_2;
  //delay(20);

  //Left Side
  
  //Setup the the Front Left leg
  fl.hipRotate.attach(FL_ROTATE_PIN);
  fl.hipLift.attach(FL_LIFT_PIN);
  fl.knee.attach(FL_KNEE_PIN);
  fl.isLeft = true;
  fl.feetPos_X = FL_X;
  fl.feetPos_Y = FL_Y;
  fl.feetPos_Z = FL_Z;
  fl.bodyOffsetX = -BODY_CENTER_OFFSET_1;
  fl.bodyOffsetY = BODY_CENTER_OFFSET_2;
  delay(20);
  //Setup the the Middle Left leg
  ml.hipRotate.attach(ML_ROTATE_PIN);
  ml.hipLift.attach(ML_LIFT_PIN);
  ml.knee.attach(ML_KNEE_PIN);
  ml.isLeft = true;
  ml.feetPos_X = ML_X;
  ml.feetPos_Y = ML_Y;
  ml.feetPos_Z = ML_Z;
  ml.bodyOffsetX = -BODY_SIDE_LENGTH;
  ml.bodyOffsetY = 0;
  delay(20);
  //Setup the the Back Left leg
  bl.hipRotate.attach(BL_ROTATE_PIN);
  bl.hipLift.attach(BL_LIFT_PIN);
  bl.knee.attach(BL_KNEE_PIN);
  bl.isLeft = true;
  bl.feetPos_X = BL_X;
  bl.feetPos_Y = BL_Y;
  bl.feetPos_Z = BL_Z;
  bl.bodyOffsetX = -BODY_CENTER_OFFSET_1;
  bl.bodyOffsetY = -BODY_CENTER_OFFSET_2;
  delay(20);
}

void moveLegIK(leg *leg, float x, float y, float z) {

  Serial.print("feetX: ");
  Serial.print(leg->feetPos_X);
  Serial.print(" | feetY: ");
  Serial.print(leg->feetPos_Y);
  Serial.print(" | feetZ: ");
  Serial.println(leg->feetPos_Z);
  
  float newX = leg->feetPos_X + x;
  float newY = leg->feetPos_Y + y;
  float newZ = leg->feetPos_Z + z;
  
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.print(z);
  Serial.print(" | newX: ");
  Serial.print(newX);
  Serial.print(" | newY: ");
  Serial.print(newY);
  Serial.print(" | newZ: ");
  Serial.println(newZ);
  
  float L1 = sqrt(powf(newX, 2) + powf(newZ, 2));
  float L2 = sqrt(powf((L1 - OFFSET_LENGTH), 2) + powf(newY, 2));

  Serial.print("L1: ");
  Serial.print(L1);
  Serial.print(" L2: ");
  Serial.println(L2);
  Serial.print("(L1 - OFESET_LENGTH)^2: ");
  Serial.print(powf((L1 - OFFSET_LENGTH), 2));
  Serial.print(" | L1 - OFESET_LENGTH: ");
  Serial.println((L1 - OFFSET_LENGTH), 2);
  
  
  //check if the leg tip can reach that point
  if(L2 > FEMUR_LENGTH + TIBIA_LENGTH){
    Serial.println("Leg can't reach that far");
    return;
  }
  
  int hipAngle = 90 - (int)(atan(newZ/newX) * 180/PI);

  float a1 = atan((L1 - OFFSET_LENGTH)/newY) * 180/PI;
  //float a1 = acos((L1 - OFFSET_LENGTH)/L2) * 180/PI;
  float a2 = acos((powf(TIBIA_LENGTH, 2) - powf(L2, 2) - powf(FEMUR_LENGTH, 2)) / (-2 * FEMUR_LENGTH * L2)) * 180/PI;
  float a3 = acos((powf(L2, 2) - powf(TIBIA_LENGTH, 2) - powf(FEMUR_LENGTH, 2)) / (-2 * TIBIA_LENGTH * FEMUR_LENGTH)) * 180/PI;

  Serial.print("a1: ");
  Serial.print(a1);
  Serial.print(" | a2: ");
  Serial.print(a2);
  Serial.print(" | a3: ");
  Serial.println(a3);
 
  
  int liftAngle = (int)(180 - (a1 + a2));
  int kneeAngle = (int)a3;

  Serial.print("hipAngle: ");
  Serial.print(hipAngle);
  Serial.print(" | liftAngle: ");
  Serial.print(liftAngle);
  Serial.print(" | kneeAngle: ");
  Serial.println(kneeAngle);
  
  moveLeg(leg, hipAngle, liftAngle, kneeAngle);
}

void moveLeg(leg *leg, int rotateAngle, int liftAngle, int kneeAngle) {
  if(leg->isLeft){
    leg->hipLift.write(180-liftAngle);
    delay(SERVO_WRITE_DELAY);
    
    leg->hipRotate.write(180-rotateAngle);
    delay(SERVO_WRITE_DELAY);

    leg->knee.write(180-kneeAngle);
    delay(SERVO_WRITE_DELAY);
  
  } else {
    leg->hipLift.write(liftAngle);
    delay(SERVO_WRITE_DELAY);
    
    leg->hipRotate.write(rotateAngle);
    delay(SERVO_WRITE_DELAY);
    
    leg->knee.write(kneeAngle);
    delay(SERVO_WRITE_DELAY);
  }
  /*
  leg.liftAngle = hipLift.read();
  delay(SERVO_WRITE_DELAY);
  leg.rotationAngle = hipRotate.read();
  delay(SERVO_WRITE_DELAY);
  leg.kneeAngle = knee.read();
  delay(SERVO_WRITE_DELAY);
  */
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
