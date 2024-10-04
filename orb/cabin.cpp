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
    if(bufIn[inWriter] == '\n') {
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