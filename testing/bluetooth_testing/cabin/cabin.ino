#include "orb.h"

Orb orb;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  orb.setup();
  delay(2000);
}

char msg[6] = "cabin\n";
char buf[256];
unsigned long prevTime = 0;
void loop() {
  // put your main code here, to run repeatedly:
  orb.receiveMessage();
  unsigned long currTime = millis() / 1000;

  if(orb.messageReady()) {
    orb.copyAndClearMessage(buf);
    Serial.println(buf);
  }

  if(currTime != prevTime) {
    prevTime = currTime;
    orb.sendMessage(msg);
    Serial.println("sending msg");
  }

  delay(10);
}
  