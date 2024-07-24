#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"

class Sensor {
  public:
    bool cabinMotionDetected();

    bool chairPressureDetected(int time);

    bool chairPressureNotDetected(int time);

    bool heartBeatDetected(int time);

    bool heartBeatNotDetected(int time);

    int getHeartBeat();
  private:
};
#endif
