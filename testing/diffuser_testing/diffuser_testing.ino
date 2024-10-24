#include "diffuser.h"
Diffuser diffuser;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  diffuser.setup();
  delay(2000);
}
int count = 0;
unsigned long startTime;
unsigned long endTime;

void loop() {
  if(count % 2 == 0) {
    count += 1;
    startTime = millis();
    Serial.println("On");
  }
  else {
    unsigned diff = millis() - startTime;
    if(diff < 4000) {
      diffuser.trigger(startTime);
    }
    else{
      count += 1;
    }
  }
}
  