//
// Created by Rls20 on 17/09/2021.
//

#ifndef Robot_Config_h
#define Robot_Config_h

// =======================
// =========CONFIG========
// =======================

// == SERVO WAIT DELAY ==
#define SERVO_WRITE_DELAY 50
// ======================

//MODE:
#define MAINTENANCE
//#define NORMAL

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
  #define BL_KNEE_INIT_ANGLE 0

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
