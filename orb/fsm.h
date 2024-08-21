#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "light.h"
#include "Constants.h"
#include "sensor.h"
#include "cabin.h"

class Fsm {
  public:
    void setup();
    void mainloop();
    void resetStates();

    void idleSetUp();
    void idleState();

    void cabinLightOnSetUp();
    void cabinLightOnState();

    void meditationFlowCheckSetUp();
    void meditationFlowCheckState();

    void meditationSetUp();
    void meditationState();
    void meditationExit();

    void endSetUp();
    void endState();
    void endExit();

    void stateChange();

  private:
    Light light;
    Sensor sensor;
    Cabin cabin;

    char buffer[15];

    //State management
    bool idle = true;
    unsigned long currTime;

    // meditationFlowCheckState
    bool meditationFlowCheck = false;

    //cabinLightsOn
    bool cabinLightOn = false;

    // meditation
    bool meditation = false;

    // end
    bool end = false;

};
#endif