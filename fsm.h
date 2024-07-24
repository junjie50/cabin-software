#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "light.h"
#include "Constants.h"

class Fsm {
  public:
    void mainloop();
    void resetStates();

    void idleSetUp();
    void idleState();

    void cabinLightOnSetUp();
    void cabinLightOnState();

    void meditationFlowCheckSetUp();
    void meditationFlowCheckState();

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

};
#endif
