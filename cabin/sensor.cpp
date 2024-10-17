#include "sensor.h"

void Sensor::polling(Orb &orb) {
  static unsigned long prevSecond = 0;
  unsigned long currSecond= (millis()/SECOND);
  unsigned long currTime = millis();
  // process data transmission from slave controller
  orb.receiveMessage();
  if(orb.messageReady()) {
    orb.copyAndClearMessage(bufIn);
  }

  // default to 0
  heartBeat = 0;
  fidget = false;

  if(strlen(bufIn) > 0) { // if there is something coming in
    if(bufIn[0] == 'b') {
      Serial.println("b");
      heartBeat = 1;
      prevBeat = currTime;
    }
    else if(bufIn[0] == 'm') {
      Serial.println("m");
      fidget = true;
      prevFidget = currTime;
    }
    bufIn[0] = '\0';
  }

  if(currSecond != prevSecond) { // poll every second
    prevSecond = currSecond; // prev second recorded
    if((currTime - prevBeat) < 1100) {
      heartBeatTime++;
      noHeartBeatTime = 0;
    }
    else{
      noHeartBeatTime++;
      heartBeatTime = 0;
    }

    if(chairPressureHigh()) {
      chairPressureTime++;
      chairNoPressureTime = 0;
    }
    else {
      chairNoPressureTime++;
      chairPressureTime = 0;
    }

    if((currTime - prevFidget) < 1100) {
      fidgetTime++;
      noFidgetTime = 0;
    }
    else{
      fidgetTime = 0;
      noFidgetTime++;
    }
  }
}

bool Sensor::cabinMotionDetected() {
  return digitalRead(PIRPIN) == HIGH;
}

bool Sensor::chairPressureDetected(int time) {
  return chairPressureTime >= time;
}

bool Sensor::chairPressureNotDetected(int time) {
  return chairNoPressureTime >= time;
}

bool Sensor::heartBeatDetected(int time) {
  return heartBeatTime >= time;
}

bool Sensor::heartBeatNotDetected(int time) {
  return noHeartBeatTime >= time;
}

bool Sensor::fidgetDetected(int time) {
  return fidgetTime >= time;
}

bool Sensor::fidgetNotDetected(int time) {
  return noFidgetTime >= time;
}



bool Sensor::chairPressureHigh() {
  return analogRead(PRESSUREPIN) > PRESSUREHIGH;
}

bool Sensor::heartBeatHigh() {
  return heartBeat > 0;
}

bool Sensor::fidgetHigh() {
  return fidget;
}

int Sensor::getHeartBeat() {
  return heartBeat;
}

void Sensor::setup() {
  pinMode(PIRPIN, INPUT); // set up PIR sensor
}








