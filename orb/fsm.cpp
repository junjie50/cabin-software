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
}






void Fsm::cabinLightOnSetUp() {
  resetStates();
  cabinLightOn = true;
  cabinLightOnStartTime = currTime;
}

// cabinLightOnState
void Fsm::cabinLightOnState() {
  // cabinLightOn Lighting
  // control glimmer at 2s interval
  light.cabinLighting();
}







void Fsm::meditationFlowCheckSetUp() {
  resetStates();
  meditationFlowCheck = true;
}

void Fsm::meditationFlowCheckState() {
  light.meditationFlowCheckLighting();
  return;
}








void Fsm::meditationSetUp() {
  resetStates();
  meditation = true;
  fidget = false;
  heartbeat = true;
}

void Fsm::meditationExit() {
  return;
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
}









void Fsm::endSetUp() {
  resetStates();
  end = true;
}


void Fsm::endExit() {
  return;
}


void Fsm::endState() {
  light.endLighting();

}





// main loop
void Fsm::setup() {
  sensor.setUp();
  light.setup();
  light.cabinLightingSetup();
}


// main loop
void Fsm::mainloop() {
  light.glimmer(MEDITATIONFLOWCHECKSTATEGLIMMER);
  // currTime = millis();
  // // sensor collect data every second and update
  // sensor.polling();

  // if(idle) {
  //   // do nothing. controll off
  //   idleState();
  // }
  // else if(cabinLightOn) {
  //   cabinLightOnState();
  // }
  // else if(meditationFlowCheck) {
  //   meditationFlowCheckState();
  // }
  // else if(meditation) {
  //   meditationState();
  // }

}