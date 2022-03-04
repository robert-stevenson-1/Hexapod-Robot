


void testMove(){
    for(int i = 0; i < 70; i+=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(i, 0, 0, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }
  for(int i = 70; i > 0; i-=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(i, 0, 0, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }
  
  for(int i = 0; i < 70; i+=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(0, i, 0, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }
  for(int i = 70; i > 0; i-=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(0, i, 0, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }

    for(int i = 0; i < 70; i+=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(0, 0, i, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }
  for(int i = 70; i > 0; i-=5){
    Serial.print("i: ");
    Serial.println(i);
    gait(0, 0, i, 0, 0, 0, -1); 
    
    moveLeg(&fr, targetAngles[0][0]+60, targetAngles[0][1], targetAngles[0][2]);
    moveLeg(&mr, targetAngles[1][0], targetAngles[1][1], targetAngles[1][2]);
    moveLeg(&br, targetAngles[2][0]-60, targetAngles[2][1], targetAngles[2][2]);
    moveLeg(&fl, targetAngles[3][0]-60, targetAngles[3][1], targetAngles[3][2]);
    moveLeg(&ml, targetAngles[4][0], targetAngles[4][1], targetAngles[4][2]);
    moveLeg(&bl, targetAngles[5][0]+60, targetAngles[5][1], targetAngles[5][2]);
    delay(500);
  }  
}
