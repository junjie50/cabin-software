#include "fsm.h"

void Fsm::resetStates() {
   bool idle = false;
   bool meditationFlowCheck = false;
   bool cabinLightOn = false;
}





void Fsm::idleSetUp() {
   bool idle = true;
   bool meditationFlowCheck = false;
   bool cabinLightOn = false;
}

void Fsm::idleState() {
  // idle state lighting system
  light.idleLighting();
  
  if(sensor.cabinMotionDeteceted()) {
    cabinLightOnSetUp();
  }
  else if(sensor.chairPressureDetected(CHAIRDETECTTIME)) {
    meditationFlowCheckSetUp();
  }
}





void Fsm::meditationFlowCheckSetUp() {
  resetStates();
  meditationFlowCheck = true;
}

void Fsm::meditationFlowCheckState() {
  continue;
}





void Fsm::cabinLightOnSetUp() {
  resetStates();
  cabinLightOn = true;
  cabinLightOnStartTime = currTime;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  // cabinLightOn Lighting
  light.cabinLightOn();

  int seconds = (cabinLightOnStartTime - currTime) / SECOND;

  // Change in state, if not remain the same
  if(seconds >= CABINLIGHTINTERVAL) {
    // check if there is movement
    if(sensor.cabinMotionDetected()) {
      cabinLightOnSetUp(); // goes back to same state
    }
    else {
      idleSetUp(); // goes back to original state
    }
  }
  else if(sensor.chairPressureDetected(CHAIRDETECTTIME)) {
    meditationFlowCheckSetUp();
  }
}





// main loop
void Fsm::mainloop() {
  currTime = millis();

  if(idle) {
    idleState();
  }
  else if(cabinLightOn) {
    cabinLightOnState();
  }
  else if(meditationFlowCheck) {
    meditationFlowCheckState();
  }

}