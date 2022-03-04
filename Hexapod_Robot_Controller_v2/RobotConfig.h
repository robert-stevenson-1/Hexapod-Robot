//
// Created by Rls20 on 17/09/2021.
//

#ifndef Robot_Config_h
#define Robot_Config_h

// =======================
// =========CONFIG========
// =======================

// == SERVO WAIT DELAY ==
#define SERVO_WRITE_DELAY 10
// ======================

// ========================
// ====ROBOT DIMENSIONS====
// ========================

#define BODY_SIDE_LENGTH 80
#define BODY_CENTER_OFFSET_1 BODY_SIDE_LENGTH/2
#define BODY_CENTER_OFFSET_2 sqrt(powf(BODY_SIDE_LENGTH, 2) - powf(BODY_CENTER_OFFSET_1, 2))

#define OFFSET_LENGTH 33 //35mm
#define FEMUR_LENGTH 90 //91mm
#define TIBIA_LENGTH 110 //113mm

//MODE:
//#define MAINTENANCE
//#define NORMAL
#define IK_MODE


// ======================
// ========LEG POS=======
// ======================

//#ifdef IK_MODE || MAINTENANCE
  #include <math.h>

// === LEG SERVO INTIAL POSITIONS ===
  //1
  #define FR_X cos(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  #define FR_Y TIBIA_LENGTH
  #define FR_Z sin(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  //2  
  #define MR_X OFFSET_LENGTH + FEMUR_LENGTH
  #define MR_Y TIBIA_LENGTH
  #define MR_Z 0
  //3
  #define BR_X cos(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  #define BR_Y TIBIA_LENGTH
  #define BR_Z sin(-60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  //6
  #define FL_X -cos(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  #define FL_Y TIBIA_LENGTH
  #define FL_Z sin(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  //5
  #define ML_X -(OFFSET_LENGTH + FEMUR_LENGTH)
  #define ML_Y TIBIA_LENGTH
  #define ML_Z 0
  //4
  #define BL_X -cos(60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)
  #define BL_Y TIBIA_LENGTH
  #define BL_Z sin(-60*PI/180)*(OFFSET_LENGTH + FEMUR_LENGTH)

  #define FR_ROTATE_INIT_ANGLE 90
  #define FR_LIFT_INIT_ANGLE 90
  #define FR_KNEE_INIT_ANGLE 90
  
  #define MR_ROTATE_INIT_ANGLE 90
  #define MR_LIFT_INIT_ANGLE 90
  #define MR_KNEE_INIT_ANGLE 90
  
  #define BR_ROTATE_INIT_ANGLE 90
  #define BR_LIFT_INIT_ANGLE 90
  #define BR_KNEE_INIT_ANGLE 90
  
  #define FL_ROTATE_INIT_ANGLE 90
  #define FL_LIFT_INIT_ANGLE 90
  #define FL_KNEE_INIT_ANGLE 90
  
  #define ML_ROTATE_INIT_ANGLE 90
  #define ML_LIFT_INIT_ANGLE 90
  #define ML_KNEE_INIT_ANGLE 90
  
  #define BL_ROTATE_INIT_ANGLE 90
  #define BL_LIFT_INIT_ANGLE 90
  #define BL_KNEE_INIT_ANGLE 90
//#endif

#ifdef NORMAL
// === LEG SERVO INTIAL ANGLES ===
  #define FR_ROTATE_INIT_ANGLE 90
  #define FR_LIFT_INIT_ANGLE 125
  #define FR_KNEE_INIT_ANGLE 110
  
  #define MR_ROTATE_INIT_ANGLE 90
  #define MR_LIFT_INIT_ANGLE 125
  #define MR_KNEE_INIT_ANGLE 110
  
  #define BR_ROTATE_INIT_ANGLE 90
  #define BR_LIFT_INIT_ANGLE 125
  #define BR_KNEE_INIT_ANGLE 110
  
  #define FL_ROTATE_INIT_ANGLE 90
  #define FL_LIFT_INIT_ANGLE 180-55
  #define FL_KNEE_INIT_ANGLE 180-70
  
  #define ML_ROTATE_INIT_ANGLE 90
  #define ML_LIFT_INIT_ANGLE 180-55
  #define ML_KNEE_INIT_ANGLE 180-70
  
  #define BL_ROTATE_INIT_ANGLE 90
  #define BL_LIFT_INIT_ANGLE 180-55
  #define BL_KNEE_INIT_ANGLE 180-70

#endif

#ifdef MAINTENANCE
// === LEG SERVO INTIAL ANGLES ===
  #define FR_ROTATE_INIT_ANGLE 90
  #define FR_LIFT_INIT_ANGLE 90
  #define FR_KNEE_INIT_ANGLE 0
  
  #define MR_ROTATE_INIT_ANGLE 90
  #define MR_LIFT_INIT_ANGLE 90
  #define MR_KNEE_INIT_ANGLE 0
  
  #define BR_ROTATE_INIT_ANGLE 90
  #define BR_LIFT_INIT_ANGLE 90
  #define BR_KNEE_INIT_ANGLE 0
  
  #define FL_ROTATE_INIT_ANGLE 90
  #define FL_LIFT_INIT_ANGLE 90
  #define FL_KNEE_INIT_ANGLE 0
  
  #define ML_ROTATE_INIT_ANGLE 90
  #define ML_LIFT_INIT_ANGLE 90
  #define ML_KNEE_INIT_ANGLE 0
  
  #define BL_ROTATE_INIT_ANGLE 90
  #define BL_LIFT_INIT_ANGLE 90
  #define BL_KNEE_INIT_ANGLE ,0

#endif

// ===============================

// === LEG SERVO PINS ===
#define FL_ROTATE_PIN 22
#define FL_LIFT_PIN 23
#define FL_KNEE_PIN 24

#define ML_ROTATE_PIN 25
#define ML_LIFT_PIN 26
#define ML_KNEE_PIN 27

#define BL_ROTATE_PIN 28
#define BL_LIFT_PIN 29
#define BL_KNEE_PIN 30

#define FR_ROTATE_PIN 31
#define FR_LIFT_PIN 32
#define FR_KNEE_PIN 33

#define MR_ROTATE_PIN 34
#define MR_LIFT_PIN 35
#define MR_KNEE_PIN 36

#define BR_ROTATE_PIN 37
#define BR_LIFT_PIN 38
#define BR_KNEE_PIN 39

// ======================

// === HEAD LEG PINS ===

#define HEAD_ROTATE_PIN 40
#define HEAD_LIFT_PIN 41

// =====================

// === ULTRASONIC PINS ===

#define ECHO_PIN 42
#define TRIG_PIN 43

// =======================


#endif
