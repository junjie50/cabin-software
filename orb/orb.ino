#include "fsm.h"
Fsm fsm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  fsm.setup();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  fsm.mainloop();
}
