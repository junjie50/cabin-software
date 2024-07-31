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

  private:
    Light light;
    Sensor sensor;

    //State management
    bool idle = true;
    unsigned long currTime;

    // meditationFlowCheckState
    bool meditationFlowCheck = false;

    //cabinLightsOn
    bool cabinLightOn = false;
    unsigned long cabinLightOnStartTime;

    // meditation
    bool meditation = false;
    bool fidget = false;
    unsigned long fidgetStartTime;
    bool heartbeat = true;

    // end
    bool end = false;

};
#endif