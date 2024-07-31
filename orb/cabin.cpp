#include "cabin.h"

void Cabin::setup() {
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Cabin::sendMessage() {
  if(BTSerial.availableForWrite() > 10) {
    Serial.write(bufOut, outWriter);
  }
}

void Cabin::receiveMessage() {
  if(BTSerial.available()) {
    bufIn[inWriter] = BTSerial.read();
    if(bufIn[inWriter] == '!') {
      msgReady = true;
      bufIn[inWriter] = '\0';
      inWriter = 0;
    }
  }
}