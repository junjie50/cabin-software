#include "fsm.h"
Fsm fsm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  fsm.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  fsm.mainloop();
}
