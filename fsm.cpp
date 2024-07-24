#include "fsm.h"

void Fsm::resetStates() {
  idle = false;
  meditationFlowCheck = false;
  cabinLightOn = false;
  meditation = false;
  end = false;
}





void Fsm::idleSetUp() {
  resetStates();
  idle = true;
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








void Fsm::cabinLightOnSetUp() {
  resetStates();
  cabinLightOn = true;
  cabinLightOnStartTime = currTime;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  // cabinLightOn Lighting
  light.cabinLighting();

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



void Fsm::meditationFlowCheckSetUp() {
  resetStates();
  meditationFlowCheck = true;
}

void Fsm::meditationFlowCheckState() {
  light.meditationFlowCheckLighting();


  if(sensor.chairPressureNotDetected(CHAIRNOTDETECTTIME)) {
    // if no pressure on chair, go to cabin light on as user not meditating.
    cabinLightOnSetUp();
  }
  else if(sensor.heartBeatDetected(HEARTBEATDETECTTIME)) {
    // if heartbeat detected for some time, go to meditation state
    meditationSetUp();
  }
}




void Fsm::meditationSetUp() {
  resetStates();
  speaker.speakerReset();
  meditation = true;
}


void Fsm::meditationState() {
  // state with ligting system
  light.meditationLighting();
  speaker.meditationSpeaker();
  diffuser.meditationDiffuser();

  if(sensor.chairPressureNotDetected(MEDITATIONNODETECTTIME) && sensor.heartBeatNotDetected(MEDITATIONNODETECTTIME)) {
    cabinLightOnSetUp();
  }
  else if(speaker.getLoopedTimes() == (SPEAKERLOOPCOUNTMINUSONE)) {
    endStateSetUp();
  }
}




void Fsm::endSetUp() {
  resetStates();
  end = true;
}


void Fsm::endState() {
  light.endLighting();
  speaker.endStateSpeaker();

  if(speaker.getLoopedTimes() == SPEAKERLOOPCOUNT && sensor.chairPressureNotDetected(ENDNOPRESSURE)) {
    cabinLightOnSetUp();
  } 
  else if(sensor.chairPressureDetected(ENDPRESSUREDETECTED)) {
    meditationFlowCheckState();
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
  else if(meditation()) {
    meditation();
  }

}