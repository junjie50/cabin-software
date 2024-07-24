#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "light.h"
#include "Constants.h"
#include "speaker.h"
#include "sensor.h"
#include "diffuser.h"

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

    void meditationSetUp();
    void meditationState();

    void endSetUp();
    void endState();

  private:
    Light light;
    Sensor sensor;
    Speaker speaker;
    Diffuser diffuser;

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

    // end
    bool end = false;

};
#endif
