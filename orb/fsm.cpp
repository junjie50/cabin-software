#include "fsm.h"

void Fsm::resetStates() {
  idle = false;
  meditationFlowCheck = false;
  cabinLightOn = false;
  meditation = false;
  end = false;
  sensor.reset();
  light.clearLight();
}

// IDLE STATE
void Fsm::idleSetUp() {
  resetStates();
  light.idleLightingSetup();
  idle = true;
}

void Fsm::idleState() {
  // idle state lighting system
  // orb do nothing
  return;
}


void Fsm::cabinLightOnSetUp() {
  resetStates();
  light.cabinLightingSetup();
  cabinLightOn = true;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  // cabinLightOn Lighting
  // control do nothing
  return;
}







void Fsm::meditationFlowCheckSetUp() {
  resetStates();
  light.meditationFlowCheckLightingSetup();
  meditationFlowCheck = true;
}

void Fsm::meditationFlowCheckState() {
  light.meditationFlowCheckLighting(sensor.controlMoved());
}








void Fsm::meditationSetUp() {
  resetStates();
  light.meditationLightingSetup();
  meditation = true;
}

void Fsm::meditationState() {
  // state with ligting system
  bool heartbeat = sensor.heartBeat();
  light.meditationLighting(heartbeat);
  sensor.triggerBuzzer(heartbeat);
}




void Fsm::endSetUp() {
  resetStates();
  end = true;
}

void Fsm::endExit() {
  return;
}

void Fsm::endState() {
  // does nothing
  return;
}

bool compareString(char *s1, char *s2) {
  int minLength = min(strlen(s1), strlen(s2));
  for(int i = 0; i < minLength; i++) {
    if(s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

void Fsm::stateChange() {
  // This part receives the update
  cabin.receiveMessage();
  if(cabin.messageReady()) {
    cabin.copyAndClearMessage(buffer);
    Serial.println(buffer);
    if(compareString(buffer, "s1") == 0) {
      idleSetUp();
      Serial.println("idle");
    }
    else if(compareString(buffer, "s2") == 0) {
      cabinLightOnSetUp();
      Serial.println("cabinlighton");
    }
    else if(compareString(buffer, "s3") == 0) {
      meditationFlowCheckSetUp();
      Serial.println("meditationflow");
    }
    else if(compareString(buffer, "s4") == 0) {
      meditationSetUp();
      Serial.println("meditation");
    }
    else if(compareString(buffer, "s5") == 0) {
      endSetUp();
      Serial.println("end");
    }
  }
}

void Fsm::sendEvent() {
  if(sensor.heartBeat()) {
    char msg[3] = "b\n";
    cabin.sendMessage(msg);
  }

  if(sensor.controlMoved()) {
    char msg[3] = "m\n";
    cabin.sendMessage(msg);
  }
}


// main loop
void Fsm::setup() {
  sensor.setUp();
  light.setup();
  cabin.setup();
  resetStates();
  idleSetUp();
}

// main loop
void Fsm::mainloop() {
  currTime = millis();
  // sensor collect data every second and update
  sensor.polling(cabin);

  // update orb state
  stateChange();

  // send out events to the cabin
  sendEvent();
  
  if(idle) {
    // do nothing. controll off
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