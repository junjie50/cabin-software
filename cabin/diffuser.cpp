#include "diffuser.h"

void Diffuser::setup() {
  pinMode(DIFFUSERPIN, OUTPUT);
}

void Diffuser::trigger(unsigned long startTime) {
  unsigned long interval = millis() - startTime;
  if(interval < 500) {
    digitalWrite(DIFFUSERPIN, HIGH);
  }
  else{
    digitalWrite(DIFFUSERPIN, LOW);
  }
}
