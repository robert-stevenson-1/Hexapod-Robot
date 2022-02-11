#include <MPU6050.h>
#include <HCSR04.h>
#include <math.h>
//#include <Servo.h>
//#include "Robot.h"
#include "LegStruct.h"
#include "RobotConfig.h"


// ======================
// =========CONST========
// ======================

#define WRITE_INTERVAL 50 //0.5 sec
#define MAX_STAGES 8

// ======================
// =========ENUMS========
// ======================

enum comID{
  FR = 1,
  MR = 2,
  BR = 3,
  FL = 6,
  ML = 5,
  BL = 4,
  R = 7
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
leg* legs[] = {&fr, &mr, &br, &fl, &ml, &bl};

int targetAngles[6][3];
bool moving = false;
bool moveDone = false;
int stage = 0;

//Comm Data
const byte maxData = 128;
char data[maxData];
char *comData[5];
char outboundData[maxData];
bool newData = false;

//Timing var for managing program blocking when writng servos
unsigned long previousMillis = 0;

//test vars
int a = 0;
bool up = true;

void setup() {
  //serial monitor
  Serial.begin(9600);
  Serial.flush();
  Serial.setTimeout(1000);

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
  #ifdef MAINTENANCE
  moveLeg(&fr, FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  moveLeg(&mr, MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  moveLeg(&br, BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  moveLeg(&fl, FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  moveLeg(&ml, ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  moveLeg(&bl, BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);
  #endif

  
  #ifdef IK_MODE
  moveLegIK(targetAngles[0], &fr, 0, 0, 0);
  moveLegIK(targetAngles[1], &mr, 0, 0, 0);
  moveLegIK(targetAngles[2], &br, 0, 0, 0);
  moveLegIK(targetAngles[3], &fl, 0, 0, 0);
  moveLegIK(targetAngles[4], &ml, 0, 0, 0);
  moveLegIK(targetAngles[5], &bl, 0, 0, 0);

  moveLeg(&fr, FR_ROTATE_INIT_ANGLE, FR_LIFT_INIT_ANGLE, FR_KNEE_INIT_ANGLE);
  moveLeg(&mr, MR_ROTATE_INIT_ANGLE, MR_LIFT_INIT_ANGLE, MR_KNEE_INIT_ANGLE);
  moveLeg(&br, BR_ROTATE_INIT_ANGLE, BR_LIFT_INIT_ANGLE, BR_KNEE_INIT_ANGLE);
  moveLeg(&fl, FL_ROTATE_INIT_ANGLE, FL_LIFT_INIT_ANGLE, FL_KNEE_INIT_ANGLE);
  moveLeg(&ml, ML_ROTATE_INIT_ANGLE, ML_LIFT_INIT_ANGLE, ML_KNEE_INIT_ANGLE);
  moveLeg(&bl, BL_ROTATE_INIT_ANGLE, BL_LIFT_INIT_ANGLE, BL_KNEE_INIT_ANGLE);
  #endif
  // ==================================

//  Serial.println("ax ay "); //az
//  Serial.println("gx gy gz");
  clearInputBuffer();
}

void loop() {
  #ifdef IK_MODE//NORMAL
  //Serial.flush();
  //used for checking intervals when moving leg servos so we only move for x amount of time minimising the amount of program blocking when writing servo positions
  unsigned long currentMillis = millis();


  //Read Data in
  getData(); //MOVE THIS TO THE ELSE OF THE IF COND BELOW    
  //after getting new data parse it as a command, only when the robot isn't in the middle of moving
  if(newData == true && moving == false){
    Serial.println(data);
    //parse the data to a command and execute the command (Command will be related to moving the robot)
    getCommand();    
  }
  
  // We are curently in the middle of moving
  if(currentMillis - previousMillis >= WRITE_INTERVAL){
    // save the last time we wrote
    previousMillis = currentMillis;
    if(moving == true){

  
      // get the next gait stage values
      gait(atoi(comData[1]), atoi(comData[2]), atoi(comData[3]), stage);
            
      // move the leg closer to their target positions. Returns true then targets are met
      if(updateLegs()){
        //Serial.println("Moving Forward point");
        /*
        Serial.print("x: ");
        Serial.print(atoi(comData[1]));
        Serial.print(" | y: ");
        Serial.print(atoi(comData[2]));
        Serial.print(" | z: ");
        Serial.print(atoi(comData[3]));
        Serial.print(" | Prev Stage: ");
        Serial.print(stage);
        Serial.print(" | Moving: ");
        Serial.print(moving);
        Serial.print(" | moveDone: ");
        Serial.print(moveDone);     
        */
        // increment the current stage in the walk cycle if all the legs have met their target positions
        stage++;
        Serial.print(" | Cur Stage: ");
        Serial.println(stage);
        //flag the whole movement operation as over by setting the moving flag to false, and seting the moveDone flag too
        if(moveDone == true){
          moving = false;
          moveDone = false;
        }
               
        // reached the end of the walk cycle and reset to stage 0 putting all the feet in the ground
        if(stage >= MAX_STAGES){
          moveDone = true; // robot is done moving
          stage = 0; // reset teh current walk cycle stage value
        }
          
      }else{
        // legs aren't at the there target so the robot is still moving
        moving = true;
      }
    }
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
  if(newData == true && moving == false){ //RESET ONLY THEN THE COMMAND IS FINISHED EXECUTING
    newData = false;
    memset(data, 0, sizeof(data)); //clear the arrays
    clearInputBuffer();
  }
  #endif 
}

void getCommand(){
    //parse data to get angles
    // Example of Command: <1 0 -25 0> (leg: 1(FR) | X: 0 | Y: -25 | Z: 0 | Stage: ~)
    // Example of Command: <8 ~ ~ ~ 0> (Gait Stage | X: ~ | Y: ~ | Z: ~ | Stage: 0)
    //  ~ : ANY INT VALUE (we don't care much for that value based on the command we are doing)
    getComData(data, comData, 5, " ");
    for(int i = 0; i < 5; i++){
      Serial.print(" | ");
      Serial.print(comData[i]);
    }
    //Execute the command that was collected
    switch(atoi(comData[0])){
      case FR: // 1
        Serial.println("FR");
        moveLegIK(targetAngles[0], &fr, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case MR: // 2
        Serial.println("MR");
        moveLegIK(targetAngles[1], &mr, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case BR: // 3
        Serial.println("BR");
        moveLegIK(targetAngles[2], &br, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case FL: // 4
        Serial.println("FL");
        moveLegIK(targetAngles[3], &fl, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case ML: // 5
        Serial.println("ML");
        moveLegIK(targetAngles[4], &ml, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case BL: // 6 
        Serial.println("BL");
        moveLegIK(targetAngles[5], &bl, atoi(comData[1]), atoi(comData[2]), atoi(comData[3]));
        break;
      case R: // 7 // Move the whole robot
        Serial.println(" R");
        gait(atoi(comData[1]), atoi(comData[2]), atoi(comData[3]), -1); 
        moveLeg(&fr, targetAngles[0][0], targetAngles[0][1], targetAngles[0][2]);
        moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
        moveLeg(&br, targetAngles[2][0], targetAngles[2][1], targetAngles[2][2]);
        moveLeg(&fl, targetAngles[3][0], targetAngles[3][1], targetAngles[3][2]);
        moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
        moveLeg(&bl, targetAngles[5][0], targetAngles[5][1], targetAngles[5][2]);
        break;
      case 8:
        Serial.println("Stage Select");
        gait(atoi(comData[1]), atoi(comData[2]), atoi(comData[3]), atoi(comData[4]));
        break;
      case 9:
        moving = true;
        break;
      default:
        Serial.println("Invalid Command ID");
        break;
    }
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
      }else{
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

bool updateLegs(){
  int tally = 0;
  //for each leg of the robot
    for (int i = 0; i < 6; i++){
      leg* l = legs[i]; //grab the address of the leg

      //hip servo of the leg
      // decide which way we are moving the servo to reach the target angle. Either Decrementing or Incrementing the current position
      if(l->curHip < targetAngles[i][0]){
        l->curHip++;
      }else if(l->curHip > targetAngles[i][0]){
        l->curHip--;
      }
      //l->hipRotate.write(l->curHip);   
      
      // Lift servo of the leg
      // decide which way we are moving the servo to reach the target angle. Either Decrementing or Incrementing the current position
      if(l->curLift < targetAngles[i][1]){
        l->curLift++;
      }else if(l->curLift > targetAngles[i][1]){
        l->curLift--;         
      }
      //l->hipLift.write(l->curLift);

      // knee servo of the leg
      // decide which way we are moving the servo to reach the target angle. Either Decrementing or Incrementing the current position
      if(l->curKnee < targetAngles[i][2]){
        l->curKnee++;
      }else if(l->curKnee > targetAngles[i][2]){
        l->curKnee--;        
      }
      //l->knee.write(l->curKnee);         

      //move the servos to the next position
      moveLeg(l, l->curHip, l->curLift, l->curKnee);

      //check if a leg has reached the target
      if (l->curHip == targetAngles[i][0] && l->curLift == targetAngles[i][1] && l->curKnee == targetAngles[i][2]){
        tally++; // increase the tally as a leg has reached its target
      }
  }

  //if the tally == 6 then all the legs have been moved to the target so we have finished moving and return false (not moving)
  if(tally == 6){
    Serial.print("Tally: ");
    Serial.println(tally);
    return true;
  }else{
    return false;
  }
  
  //Serial.println("Done updating Servos");
}

void gait(int x, int y, int z, int stage){
  switch(stage){
    case 0:
      Serial.println("All legs down (Reset)");
      //set 1
      moveLegIK(targetAngles[0], &fr, 0, 0, 0);
      moveLegIK(targetAngles[2], &br, 0, 0, 0);
      moveLegIK(targetAngles[4], &ml, 0, 0, 0);
      //set 2
      moveLegIK(targetAngles[1], &mr, 0, 0, 0);
      moveLegIK(targetAngles[3], &fl, 0, 0, 0);
      moveLegIK(targetAngles[5], &bl, 0, 0, 0); 
      break;
    case 1:
      Serial.println("Set 1 up");
      //set 1
      moveLegIK(targetAngles[0], &fr, 0, 50 + y, 0);
      moveLegIK(targetAngles[2], &br, 0, 50 + y, 0);
      moveLegIK(targetAngles[4], &ml, 0, 50 + y, 0);
      break;
    case 2: 
      Serial.println("Set 1 up move");
      //set 1
      Serial.println("Fr Leg");
      moveLegIK(targetAngles[0], &fr, x, 50 + y, z);
      moveLegIK(targetAngles[2], &br, x, 50 + y, z);
      moveLegIK(targetAngles[4], &ml, x, 50 + y, z);
      break;
    case 3: 
      Serial.println("Set 1 down");
      //set 1
      moveLegIK(targetAngles[0], &fr, x, 0, z);
      moveLegIK(targetAngles[2], &br, x, 0, z);
      moveLegIK(targetAngles[4], &ml, x, 0, z);
      break;
    case 4: 
      Serial.println("Set 2 up");
      //set 2
      moveLegIK(targetAngles[1], &mr, 0, 50 + y, 0);
      moveLegIK(targetAngles[3], &fl, 0, 50 + y, 0);
      moveLegIK(targetAngles[5], &bl, 0, 50 + y, 0);
      break;
    case 5: 
      Serial.println("Set 1 down move");
      //set 1
      moveLegIK(targetAngles[0], &fr, 0, 0, 0);
      moveLegIK(targetAngles[2], &br, 0, 0, 0);
      moveLegIK(targetAngles[4], &ml, 0, 0, 0);
      break;
    case 6: 
      Serial.println("Set 2 up move");
      //set 2
      moveLegIK(targetAngles[1], &mr, x, 50 + y, z);
      moveLegIK(targetAngles[3], &fl, x, 50 + y, z);
      moveLegIK(targetAngles[5], &bl, x, 50 + y, z);
      break; 
    case 7:
      Serial.println("Set 2 Down");
      //set 2
      moveLegIK(targetAngles[1], &mr, x, 0, z);
      moveLegIK(targetAngles[3], &fl, x, 0, z);
      moveLegIK(targetAngles[5], &bl, x, 0, z);
      break;   
    default:
      //set 2
      moveLegIK(targetAngles[1], &mr, x, y, z);
      moveLegIK(targetAngles[3], &fl, x, y, z);
      moveLegIK(targetAngles[5], &bl, x, y, z);
      //set 1
      moveLegIK(targetAngles[0], &fr, x, y, z);
      moveLegIK(targetAngles[2], &br, x, y, z);
      moveLegIK(targetAngles[4], &ml, x, y, z);
      break;
  }
}

//Returns the angle that the leg's servos have to be set to reach that point in the retData param
void moveLegIK(int *retData, leg *leg, float x, float y, float z) {
/*
  Serial.print("feetX: ");
  Serial.print(leg->feetPos_X);
  Serial.print(" | feetY: ");
  Serial.print(leg->feetPos_Y);
  Serial.print(" | feetZ: ");
  Serial.println(leg->feetPos_Z);
*/
  float newX = leg->feetPos_X + x;
  float newY = leg->feetPos_Y + y;
  float newZ = leg->feetPos_Z + z;
  /*
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
  */
  float L1 = sqrt(powf(newX, 2) + powf(newZ, 2));
  float L2 = sqrt(powf((L1 - OFFSET_LENGTH), 2) + powf(newY, 2));
/*
  Serial.print("L1: ");
  Serial.print(L1);
  Serial.print(" L2: ");
  Serial.println(L2);
  Serial.print("(L1 - OFESET_LENGTH)^2: ");
  Serial.print(powf((L1 - OFFSET_LENGTH), 2));
  Serial.print(" | L1 - OFESET_LENGTH: ");
  Serial.println((L1 - OFFSET_LENGTH), 2);
*/  
  
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
/*
  Serial.print("a1: ");
  Serial.print(a1);
  Serial.print(" | a2: ");
  Serial.print(a2);
  Serial.print(" | a3: ");
  Serial.println(a3);
*/ 
  
  int liftAngle = (int)(180 - (a1 + a2));
  int kneeAngle = (int)a3;
//
//  Serial.print("hipAngle: ");
//  Serial.print(hipAngle);
//  Serial.print(" | liftAngle: ");
//  Serial.print(liftAngle);
//  Serial.print(" | kneeAngle: ");
//  Serial.println(kneeAngle);


/*  
  Serial.print("BEFORE =>");
  Serial.print("retData[0]: ");
  Serial.print(retData[0]);
  Serial.print(" | retData[1]: ");
  Serial.print(retData[1]);
  Serial.print(" | retData[2]: ");
  Serial.println(retData[2]);
*/  
  retData[0] = hipAngle;
  retData[1] = liftAngle;
  retData[2] = kneeAngle;
/*  
  Serial.print("AFTER =>");
  Serial.print("retData[0]: ");
  Serial.print(retData[0]);
  Serial.print(" | retData[1]: ");
  Serial.print(retData[1]);
  Serial.print(" | retData[2]: ");
  Serial.println(retData[2]);
*/
}

void moveLeg(leg *leg, int rotateAngle, int liftAngle, int kneeAngle) {
  if(leg->isLeft){
    leg->hipLift.write(180-liftAngle);

    leg->hipRotate.write(rotateAngle);

    leg->knee.write(180-kneeAngle);
  
  } else {
    leg->hipLift.write(liftAngle);
   
    leg->hipRotate.write(rotateAngle);
    
    leg->knee.write(kneeAngle);
  }

  leg->curHip = rotateAngle; 
  leg->curLift = liftAngle;
  leg->curKnee = kneeAngle;
  
  /*
  leg.liftAngle = hipLift.read();
  delay(SERVO_WRITE_DELAY);
  leg.rotationAngle = hipRotate.read();
  delay(SERVO_WRITE_DELAY);
  leg.kneeAngle = knee.read();
  delay(SERVO_WRITE_DELAY);
  */
}

void getBalanceData() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //ax = map(ax, -17000, 17000, 0, 255 ); // X axis data
  //ay = map(ay, -17000, 17000, 0, 255); // Y axis data
  //az = map(az, -17000, 17000, 0, 255);  // Z axis data
}
