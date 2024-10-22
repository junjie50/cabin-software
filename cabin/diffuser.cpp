#include "diffuser.h"

void Diffuser::setup() {
  pinMode(DIFFUSERPIN, OUTPUT);
}

void Diffuser::trigger(unsigned long startTime, bool start) {
  unsigned long interval = millis() - startTime;
  if(start) {
    if(interval < DIFFUSERDELAY) {
      return;
    }
    else if(interval < DIFFUSERDELAYTRIGGER) {
      digitalWrite(DIFFUSERPIN, HIGH);
    }
    else{
      digitalWrite(DIFFUSERPIN, LOW);
    }
  }
  else {
    if(interval < 1000) { // trigger for 1 second to test
      digitalWrite(DIFFUSERPIN, HIGH);
    }
    else{
      digitalWrite(DIFFUSERPIN, LOW);
    }
  }
}
