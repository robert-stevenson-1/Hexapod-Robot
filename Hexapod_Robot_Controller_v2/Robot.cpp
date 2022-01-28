//
// Created by Rls20 on 17/09/2021.
//

#include "Robot.h"

Robot::Robot() {
  /*
    fr = Leg(FR_ROTATE_PIN, FR_LIFT_PIN, FR_KNEE_PIN, false);
    mr = Leg(MR_ROTATE_PIN, MR_LIFT_PIN, MR_KNEE_PIN, false);
    br = Leg(BR_ROTATE_PIN, BR_LIFT_PIN, BR_KNEE_PIN, false);
    fl = Leg(FL_ROTATE_PIN, FL_LIFT_PIN, FL_KNEE_PIN, true);
    ml = Leg(ML_ROTATE_PIN, ML_LIFT_PIN, ML_KNEE_PIN, true);
    bl = Leg(BL_ROTATE_PIN, BL_LIFT_PIN, BL_KNEE_PIN, true);
  */
}

void Robot::updateLeg(Leg *leg, int rotateAngle, int liftAngle, int kneeAngle) {
    leg->moveLeg(rotateAngle,liftAngle,kneeAngle);
}

Leg &Robot::getFr() {
    return fr;
}

Leg &Robot::getFl() {
    return fl;
}

Leg &Robot::getMr() {
    return mr;
}

Leg &Robot::getMl() {
    return ml;
}

Leg &Robot::getBr() {
    return br;
}

Leg &Robot::getBl() {
    return bl;
}

//void &getRobotAngles(char* data){
//  data = fr.getRotationAngle() + ' ' + fr.getLiftAngle() + ' ' + fr.getKneeAngle() + ' ' +
//  mr.getRotationAngle() + ' ' + mr.getLiftAngle() + ' ' + mr.getKneeAngle() + ' ' +
//  br.getRotationAngle() + ' ' + br.getLiftAngle() + ' ' + br.getKneeAngle() + ' ' +
//  fl.getRotationAngle() + ' ' + fl.getLiftAngle() + ' ' + fl.getKneeAngle() + ' ' +
//  ml.getRotationAngle() + ' ' + ml.getLiftAngle() + ' ' + ml.getKneeAngle() + ' ' +
//  bl.getRotationAngle() + ' ' + bl.getLiftAngle() + ' ' + bl.getKneeAngle() + '\0';
//}
