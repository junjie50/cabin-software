#include "diffuser.h"

void Diffuser::setup() {
  pinMode(51, OUTPUT);
}

void Diffuser::trigger(unsigned long startTime) {
  unsigned long interval = millis() - startTime;
  if(interval < 500) {
    digitalWrite(51, HIGH);
  }
  else{
    digitalWrite(51, LOW);
  }
}
