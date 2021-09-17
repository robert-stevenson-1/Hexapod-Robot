//
// Created by Rls20 on 17/09/2021.
//

#ifndef ROBOT___C___ROBOT_H
#define ROBOT___C___ROBOT_H


#include "Leg.h"
#include "RobotConfig.h"

class Robot {

private:
    Leg fr;
    Leg fl;
    Leg mr;
    Leg ml;
    Leg br;
    Leg bl;
public:
    Robot();
    void updateLeg(Leg *leg, int rotateAngle, int liftAngle, int kneeAngle);
    Leg &getFr();
    Leg &getFl();
    Leg &getMr();
    Leg &getMl();
    Leg &getBr();
    Leg &getBl();

};


#endif //ROBOT_H
