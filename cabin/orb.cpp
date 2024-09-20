#include "orb.h"

// GET MESSAGE FUNCTIONS
bool Orb::messageReady() {
  return msgReady;
}

void Orb::getMessage(char *buf) {
  memcpy(buf, bufIn, strlen(bufIn) + 1);
}


void Orb::setup() {
  Serial1.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Orb::sendMessage(char *msg) {
  Serial1.println(msg);
}

void Orb::receiveMessage(char *buf) {
  int start = 0;
  buf[start] = '\0';
  if(Serial1.available()) {
    Serial.println("avail");
    buf[start++] = Serial1.read();
    while(Serial1.available()) {
      buf[start++] = Serial1.read();
    }
  }
}