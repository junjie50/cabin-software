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
  Serial.println("idle");
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
  Serial.println("light on");
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
  Serial.println("meditation flow");
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
  speaker.meditationSpeaker();
  meditationStart  = currTime;
  diffuserStartTime = currTime;
  diffuserStart = true;
  meditation = true;
  fidget = false;
  heartbeat = true;
  Serial.println("meditation");
}

void Fsm::meditationExit() {
  diffuserEndTime = millis();
  diffuserStart = false;
}

void Fsm::meditationState() {
  static unsigned long duration = 30000;
  // state with ligting system according to the heartbeat that is recorded every second
  unsigned long diff = currTime - meditationStart;
  light.meditationLighting(sensor.getHeartBeat());

  // State changes
  if(sensor.chairPressureNotDetected(MEDITATIONNODETECTTIME) && sensor.heartBeatNotDetected(MEDITATIONNODETECTTIME)) { // go to cabin
    speaker.speaker1Stop();
    speaker.speaker2Stop();
    meditationExit();
    cabinLightOnSetUp();
  }
  else if(diff > duration) { // go to end state
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
        Serial.println("speaker started");
        speaker.fidgetSpeakerStart();
      }
    }
    else if(sensor.fidgetNotDetected(FIDGETDETECTTIME)) {
      Serial.println("fidget stopped");
      fidget = false;
      speaker.speaker2Stop();
    }
    else{
      speaker.fidgetStateSpeaker(fidgetStartTime); // change the volume
    }
  }
}







void Fsm::endSetUp() {
  Serial.println("end");
  endStart = currTime;
  resetStates();
  char msg[4] = "s5\n";
  orb.sendMessage(msg);
  end = true;
}

void Fsm::endState() {
  static unsigned long duration = 30000;
  static unsigned long timeToLeave = 10000;
  light.endLighting();
  unsigned long timeDiff = currTime - endStart;
  if(timeDiff > timeToLeave) { // give people time to react
    if(sensor.chairPressureNotDetected(ENDNOPRESSURE) && !sensor.itemInBase()) { // never detect chair for 5 seconds to exit state
      speaker.speaker1Stop();
      speaker.speaker2Stop();
      cabinLightOnSetUp();
    }
    else if(sensor.chairPressureDetected(ENDPRESSUREDETECTED)) { // has been sitting for the pass 1 minute, continue with meditation.
      meditationSetUp();
    }
  }

  light.baseLighting(sensor.itemInBase());
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
    diffuser.trigger(diffuserStartTime, true);
  }
  else {
    diffuser.trigger(diffuserEndTime, false);
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
  }
  else if(end) {
    endState();
  }
}