#include "orb.h"

// GET MESSAGE FUNCTIONS
bool Orb::messageReady() {
  return msgReady;
}

void Orb::getMessage(char *buf) {
  memcpy(buf, bufIn, strlen(bufIn) + 1);
}


void Orb::setup() {
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Orb::sendMessage() {
  if(BTSerial.availableForWrite() > 10) {
    BTSerial.write(bufOut, outWriter);
    outWriter = 0;
  }
}

void Orb::receiveMessage() {
  if(BTSerial.available() && !msgReady) {
    bufIn[inWriter] = BTSerial.read();
    if(bufIn[inWriter] == '!') {
      msgReady = true;
      bufIn[inWriter] = '\0';
      inWriter = 0;
    }
  }
}