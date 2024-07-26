#include "sensor.h"

void Sensor::polling() {
  unsigned long currTime = (millis()/SECOND);
  if(currTime != prevTime) {
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