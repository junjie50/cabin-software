#include "cabin.h"

void Cabin::setup() {
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Cabin::sendMessage(char *msg) {
    BTSerial.write(msg, strlen(msg));
}

// Clears the buffer everytime it is being called.
void Cabin::receiveMessage() {
  if(BTSerial.available() && !msgReady) {
    bufIn[inWriter] = BTSerial.read();
    if(inWriter == 0 && bufIn[inWriter] != 's'){ // invalid first character
      if(bufIn[inWriter] > '0' && bufIn[inWriter] <= '5'){
        bufIn[1] = bufIn[0];
        bufIn[0] = 's';
        inWriter = 2;
      }
    }
    else if(bufIn[inWriter] == '\n') {
      bufIn[inWriter] = 0;
      inWriter = 0;
      msgReady = true;
    }
    else {
      inWriter++;
    }
  }
}


void Cabin::copyAndClearMessage(char *buffer) {
  strcpy(buffer, bufIn);
  msgReady = false;
}

bool Cabin::messageReady(){
  return msgReady;
}