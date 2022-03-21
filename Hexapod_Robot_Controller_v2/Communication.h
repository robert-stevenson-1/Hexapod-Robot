#ifndef Communication_h
#define Communication_h

//Comm Data
const byte maxData = 128;
char data[maxData];
char *comData[8];
char outboundData[maxData];
bool newData = false;

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

void getComData(char* data, char** retData, const int arrSize, const char* delim){

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

#endif
