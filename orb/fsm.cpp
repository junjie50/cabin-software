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


// main loop
void Fsm::setup() {
  sensor.setUp();
  light.setup();
  cabin.setup();
  resetStates();
  idleSetUp();
}

char data[] = "20,0";
// main loop
void Fsm::mainloop() {
  currTime = millis();
  // sensor collect data every second and update
  while(true) {
    cabin.sendMessage(data);
    delay(1000);
  }

  sensor.polling(cabin);
  //if there is sensor pulse, forward it for cabin
  stateChange();
  
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