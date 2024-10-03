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

void Fsm::stateChange() {
  // This part receives the update
  cabin.receiveMessage(buffer);

  if(strcmp(buffer, "s1") == 0) {
    idleSetUp();
  }
  else if(strcmp(buffer, "s2") == 0) {
    cabinLightOnSetUp();
  }
  else if(strcmp(buffer, "s3") == 0) {
    meditationFlowCheckSetUp();
  }
  else if(strcmp(buffer, "s4") == 0) {
    meditationSetUp();
  }
  else if(strcmp(buffer, "s5") == 0) {
    endSetUp();
  }
}

void Fsm::sendEvent() {
  if(sensor.heartBeat()) {
    buffer[0] = 'b';
    buffer[1] = '\n';
    cabin.sendMessage(buffer);
  }
  else if(sensor.controlMoved()) {
    buffer[0] = 'm';
    buffer[1] = '\n';
    cabin.sendMessage(buffer);
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
    Serial.println("idle");
  }
  else if(cabinLightOn) {
    cabinLightOnState();
    Serial.println("cabinlight");
  }
  else if(meditationFlowCheck) {
    meditationFlowCheckState();
    Serial.println("meditationflow");
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