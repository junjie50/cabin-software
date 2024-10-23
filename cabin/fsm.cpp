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
  idle = true;
  msgCount = 0;
}

void Fsm::idleState() {
  // repeat blueooth state change message
  unsigned long diff = currTime - prevSend;
  if(msgCount < 3 && diff > 300) {
    orb.sendMessage("s1\n");
    prevSend = currTime;
    msgCount++;
  }
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
  cabinLightOn = true;
  cabinLightOnStartTime = currTime;
  msgCount = 0;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  unsigned long diff = currTime - prevSend;
  if(msgCount < 3 && diff > 300) {
    orb.sendMessage("s2\n");
    prevSend = currTime;
    msgCount++;
  }
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
  light.meditationFlowCheckLighting(true);
  meditationFlowCheck = true;
  msgCount = 0;
}

void Fsm::meditationFlowCheckState() {
  unsigned long diff = currTime - prevSend;
  if(msgCount < 3 && diff > 300) {
    orb.sendMessage("s3\n");
    prevSend = currTime;
    msgCount++;
  }

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
  speaker.meditationSpeaker();
  meditationStart  = currTime;
  diffuserStartTime = currTime;
  diffuserStart = true;
  meditation = true;
  fidget = false;
  heartbeat = true;
  Serial.println("meditation");
  msgCount = 0;
}

void Fsm::meditationExit() {
  speaker.speaker1Stop();
  speaker.speaker2Stop();
  diffuserEndTime = millis();
  diffuserStart = false;
}

void Fsm::meditationState() {
  unsigned long diffMsg = currTime - prevSend;
  if(msgCount < 3 && diffMsg > 300) {
    orb.sendMessage("s4\n");
    prevSend = currTime;
    msgCount++;
  }
  static unsigned long duration = 30000;
  // state with ligting system according to the heartbeat that is recorded every second
  unsigned long diff = currTime - meditationStart;
  light.meditationLighting(sensor.getHeartBeat());

  // State changes
  if(sensor.chairPressureNotDetected(MEDITATIONNODETECTTIME) && sensor.heartBeatNotDetected(MEDITATIONNODETECTTIME)) { // go to cabin
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
        Serial.println("Fidget speaker");
        speaker.fidgetSpeakerStart();
      }
    }
    else if(sensor.fidgetNotDetected(FIDGETNOTDETECTTIME)) {
      Serial.println("Fidget stopped");
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
  end = true;
  msgCount = 0;
}

void Fsm::endState() {
  static unsigned long duration = 30000;
  static unsigned long timeToLeave = 10000;
  unsigned long diff = currTime - prevSend;
  if(msgCount < 3 && diff > 300) {
    orb.sendMessage("s5\n");
    prevSend = currTime;
    msgCount++;
  }
  light.endLighting();
  unsigned long timeDiff = currTime - endStart;
  if(timeDiff > timeToLeave) { // give people time to react
    if(sensor.chairPressureNotDetected(ENDNOPRESSURE) && sensor.itemInBase()) { // never detect chair for 5 seconds to exit state
      light.baseOff(); // turn off the base light
      cabinLightOnSetUp(); // go to light on state
    }
    else if(sensor.chairPressureDetected(ENDPRESSUREDETECTED)) { // has been sitting for the pass 1 minute, continue with meditation.
      light.baseOff(); // turn off the base light
      meditationSetUp();
    }
  }

  light.baseLighting(sensor.itemInBase()); // once item in base, base will turn off
  // repeat blueooth state change message
  if(msgCount < 2) {
    orb.sendMessage("s5\n");
    msgCount++;
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