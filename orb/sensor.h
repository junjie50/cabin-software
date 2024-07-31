#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "Constants.h"
#include "DFRobot_Heartrate.h"

class Sensor {
  public:
    void setUp();
    void polling();

    // HEART BEAT SENSOR OPERATIONS
    bool heartBeatDetected(int time);
    bool heartBeatNotDetected(int time);
    int getHeartBeat();

    // GYRO SENSOR DETECT MOVEMENT
    bool controlMoved();

  private:
    unsigned long prevTime = 0;
    int heartBeatTime = 0;
    int noHeartBeatTime = 0;
    DFRobot_Heartrate heartrate{DIGITAL_MODE};

};
#endif