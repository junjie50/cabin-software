#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "Constants.h"
#include "DFRobot_Heartrate.h"
#include "orb.h"

class Sensor {
  public:
    void setup();

    void polling(Orb orb);

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
    // Record our own heartbeat as it is tracked by another processor
    // Every second Serial comms to send the data
    int heartBeat = 0; 
    bool fidget = false; 

    unsigned long prevBeat = 0;
    unsigned long prevFidget = 0;


    // duration tracker for state change
    int chairPressureTime = 0;
    int chairNoPressureTime = 0;
    int heartBeatTime = 0;
    int noHeartBeatTime = 0;
    int fidgetTime = 0;

    // buffer for message
    char bufIn[15];
};
#endif
