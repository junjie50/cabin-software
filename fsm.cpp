#include "fsm.h"

void Fsm::resetStates() {
  idle = false;
  meditationFlowCheck = false;
  cabinLightOn = false;
  meditation = false;
  end = false;
}

// IDLE STATE
void Fsm::idleSetUp() {
  resetStates();
  idle = true;
}

void Fsm::idleState() {
  // idle state lighting system
  light.idleLighting();
  
  if(sensor.cabinMotionDetected()) {
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
  diffuser.start();
  meditation = true;
  fidget = false;
  heartbeat = true;
}

void Fsm::meditationExit() {
  speaker.speaker2Stop();
  speaker.speaker1Stop();
  diffuser.stop();
}



void Fsm::meditationState() {
  // state with ligting system
  if(!heartbeat) {
    light.meditationNoHBLighting(); // average human hb
    // buzz
    // control like static soft pink
  }
  else {
    light.meditationLighting();
  }

  // additional speaker
  if(fidget){
    speaker.fidgetStateSpeaker();
  }
  else {
    speaker.speaker2Stop(); 
  }

  speaker.meditationSpeaker();

  if(sensor.chairPressureNotDetected(MEDITATIONNODETECTTIME) && sensor.heartBeatNotDetected(MEDITATIONNODETECTTIME)) {
    meditationExit();
    cabinLightOnSetUp();
  }
  else if(speaker.getLoopedTimes() == SPEAKERLOOPCOUNTMINUSONE) {
    meditationExit();
    endSetUp();
  }
  else {
    // heart beat check and change state
    if(sensor.heartBeatNotDetected(CHAIRDETECTTIME) && sensor.chairPressureDetected(CHAIRDETECTTIME)){
      // buzz
      heartbeat = false;
    }
    else{
      heartbeat = true;
    }

    // fidget check and change state
    if(!fidget) {
      if(sensor.fidgetDetected(FIDGETDETECTTIME)) {
        fidget = true;
        fidgetStartTime = currTime;
      }
    }
    else if ((currTime - fidgetStartTime) / SECOND > 5){ // no fidget for more than 5 seconds
      if(!sensor.fidgetDetected(FIDGETDETECTTIME)) {
        fidget = false;
      }
    }
  }
}




void Fsm::endSetUp() {
  resetStates();
  end = true;
}


void Fsm::endExit() {
  speaker.speaker1Stop();
  speaker.speaker2Stop();
}


void Fsm::endState() {
  light.endLighting();
  speaker.endStateSpeaker();

  if(speaker.getLoopedTimes() == SPEAKERLOOPCOUNT && sensor.chairPressureNotDetected(ENDNOPRESSURE)) {
    endExit();
    cabinLightOnSetUp();
  } 
  else if(sensor.chairPressureDetected(ENDPRESSUREDETECTED)) {
    endExit();
    meditationFlowCheckState();
  }
}


// main loop
void Fsm::setup() {
  speaker.speakerSetUP(); // set up speaker serial and everything
}


// main loop
void Fsm::mainloop() {
  currTime = millis();
  // sensor collect data every second and update
  sensor.polling();

  if(idle) {
    idleState();
  }
  else if(cabinLightOn) {
    cabinLightOnState();
  }
  else if(meditationFlowCheck) {
    meditationFlowCheckState();
  }
  else if(meditation) {
    meditationState();
  }

}