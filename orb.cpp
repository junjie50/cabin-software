#include "orb.h"

void Orb::setUp() {
  BTSerial = SoftwareSerial(RXPIN, TXPIN);
  BTSerial.begin(38400); // HC-05 default speed in AT command mode is 38400
}

void Orb::sendMessage(String data) {
  data.toCharArray(buffer,2);
  BTSerial.print(buffer);
}