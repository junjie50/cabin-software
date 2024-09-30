#include "cabin.h"

void Cabin::setup() {
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Cabin::sendMessage(char *msg) {
    BTSerial.print(msg);
}

// Clears the buffer everytime it is being called.
void Cabin::receiveMessage(char *buf) {
  int start = 0;
  buf[start] = '\n';
  if(BTSerial.available()) {
    buf[start++] = BTSerial.read();
    while(BTSerial.available()) {
      buf[start++] = BTSerial.read();
    }
    buf[start] = '\0';
  }
}


void Cabin::copyAndClearMessage(char *buffer) {
  strcpy(buffer, bufIn);
  msgReady = false;
}

bool Cabin::messageReady(){
  return msgReady;
}