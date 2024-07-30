#include "sensor.h"

void Sensor::polling(Orb orb) {
  static unsigned long prevTime = 0;
  unsigned long currTime = (millis()/SECOND);
  // process data transmission from slave controller
  orb.receiveMessage();
  if(orb.messageReady()) {
    orb.getMessage(bufIn);
    char *res = strtok(bufIn, ',');
    int count = 0;
    while(res != NULL) {
      int curr = atoi(res);
      if(count == 0) {
        heartBeat = curr;
      }
      else {
        if(curr == 1) {
          fidget = true;
        }
        else{
          fidget = false;
        }
      }
      count++;
      res = strtok(NULL, ',');
    }
  }

  if(currTime != prevTime) { // poll every second
    prevTime = currTime; // prev second recorded
    if(heartBeatHigh()) {
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

    if(fidgetHigh()) {
      fidgetTime++;
    }
    else{
      fidgetTime = 0;
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








