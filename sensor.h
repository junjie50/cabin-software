#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "Constants.h"

class Sensor {
  public:
    void polling();

    bool cabinMotionDetected();

    bool chairPressureDetected(int time);

    bool chairPressureNotDetected(int time);

    bool heartBeatDetected(int time);

    bool heartBeatNotDetected(int time);

    bool fidgetDetected(int time);

    int getHeartBeat();




    bool chairPressureHigh();
    bool heartBeatHigh();
    bool fidgetHigh();

  private:
    int chairPressureTime = 0;
    int chairNoPressureTime = 0;
    int heartBeatTime = 0;
    int noHeartBeatTime = 0;
    int fidgetTime = 0;
    unsigned long prevTime = 0;
};
#endif
