#include "orb.h"

void Orb::setup() {
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Orb::sendMessage(char *msg) {
  BTSerial.write(msg, strlen(msg));
}

void Orb::receiveMessage() {
  if(BTSerial.available() && !msgReady) {
    //Serial.println("Received one letter");
    bufIn[inWriter] = BTSerial.read();
    if(bufIn[inWriter] == '\n') {
      bufIn[inWriter] = 0;
      inWriter = 0;
      msgReady = true; // indicate true to stop writing to it
    }
    else {
      inWriter++;
    }
  }
}

void Orb::copyAndClearMessage(char *buffer) {
  strcpy(buffer, bufIn);
  msgReady = false;
}

bool Orb::messageReady(){
  return msgReady;
}