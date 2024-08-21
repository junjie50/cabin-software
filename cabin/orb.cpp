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

void Orb::sendMessage(char *msg) {
  BTSerial.println(msg);
}

void Orb::receiveMessage(char *buf) {
  int start = 0;
  buf[start] = '\0';
  if(BTSerial.available()) {
    buf[start++] = BTSerial.read();
    while(BTSerial.available()) {
      buf[start++] = BTSerial.read();
    }
  }
}