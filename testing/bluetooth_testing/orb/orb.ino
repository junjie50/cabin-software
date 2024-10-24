#include "cabin.h"

Cabin cabin;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  cabin.setup();
  delay(2000);
}


char msg[5] = "orbs\n";
char buf[256];
unsigned long prevTime = 0;
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currTime = millis() / 1000;
  cabin.receiveMessage();

  if(cabin.messageReady()) {
    cabin.copyAndClearMessage(buf);
    Serial.println(buf);
  }

  if(currTime != prevTime){
    prevTime = currTime;
    cabin.sendMessage(msg);
    Serial.println("sending msg");
  }
  
  delay(10);
}
