#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"

class Sensor {
  public:
    bool cabinMotionDetected();

    bool chairPressureDetected(int time);

    bool heartBeatDetected();

    int getHeartBeat();
  private:
};
#endif
