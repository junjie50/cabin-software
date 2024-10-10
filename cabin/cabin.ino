#include "fsm.h"

Fsm fsm;

void setup() {
  // put your setup code here, to run once:
  pinMode( 15, INPUT_PULLUP );
  pinMode( 17, INPUT_PULLUP );
  pinMode( 19, INPUT_PULLUP );
  Serial.begin(115200);
  fsm.setup();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  fsm.mainloop();
}
  