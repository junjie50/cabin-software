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
  char msg[4] = "s1\n";
  orb.sendMessage(msg);
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
  char msg[4] = "s2\n";
  orb.sendMessage(msg);
  cabinLightOn = true;
  cabinLightOnStartTime = currTime;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  // cabinLightOn Lighting
  light.cabinLighting();

  int seconds = (currTime - cabinLightOnStartTime) / SECOND;

  if(sensor.cabinMotionDetected()) {
    cabinLightOnStartTime = currTime;
  }

  // Change in state, if not remain the same
  if(seconds >= CABINLIGHTINTERVAL) {
    // check if there is movement
    idleSetUp(); // goes back to original state
  }
  else if(sensor.chairPressureDetected(CHAIRDETECTTIME)) {
    meditationFlowCheckSetUp();
  }
}







void Fsm::meditationFlowCheckSetUp() {
  resetStates();
  char msg[4] = "s3\n";
  orb.sendMessage(msg);
  light.meditationFlowCheckLighting(true);
  meditationFlowCheck = true;
}

void Fsm::meditationFlowCheckState() {
  light.meditationFlowCheckLighting(false);

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
  char msg[4] = "s4\n";
  orb.sendMessage(msg);
  speaker.speakerReset();
  diffuserStartTime = millis();
  diffuserStart = true;
  meditation = true;
  fidget = false;
  heartbeat = true;
}

void Fsm::meditationExit() {
  speaker.speaker2Stop();
  speaker.speaker1Stop();
  diffuserEndTime = millis();
  diffuserStart = false;
}

void Fsm::meditationState() {
  // state with ligting system according to the heartbeat that is recorded every second
  light.meditationLighting(sensor.getHeartBeat());

  // additional speaker
  if(fidget){
    speaker.fidgetStateSpeaker(fidgetStartTime);
  }
  else {
    speaker.speaker2Stop(); 
  }

  speaker.meditationSpeaker();

  // State changes
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
  char msg[4] = "s5\n";
  orb.sendMessage(msg);
  end = true;
}


void Fsm::endExit() {
  speaker.speaker1Stop();
  speaker.speaker2Stop();
}


void Fsm::endState() {
  light.endLighting();
  speaker.endStateSpeaker();

  if(speaker.getLoopedTimes() >= SPEAKERLOOPCOUNT && sensor.chairPressureNotDetected(ENDNOPRESSURE)) {
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
  orb.setup(); //setup bluetooth comms with orb
  sensor.setup();
  diffuser.setup();
  speaker.speakerSetUP(); // set up speaker serial and everything
  light.setup();
  idleSetUp();
}

void Fsm::realtime() {
  if(diffuserStart) {
    diffuser.trigger(diffuserStartTime);
  }
  else {
    diffuser.trigger(diffuserEndTime);
  }
}

int count = 0;
char buf[256];
// main loop
void Fsm::mainloop() {
  currTime = millis(); // system clock
  // sensor collect data every second and 
  // updates include those from orb if orb has sent hb and fidget

  sensor.polling(orb);
  
  realtime();

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
    Serial.println("meditation");
  }
  else if(end) {
    endState();
    Serial.println("end");
  }
}