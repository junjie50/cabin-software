#ifndef fsmClass_h
#define fsmClass_h
#include "Arduino.h"
#include "light.h"
#include "Constants.h"
#include "speaker.h"
#include "sensor.h"
#include "diffuser.h"
#include "orb.h"

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

    void realtime();
    void endSetUp();
    void endState();
    void endExit();

  private:
    Light light;
    Sensor sensor;
    Speaker speaker;
    Diffuser diffuser;
    Orb orb;

    //State management
    bool idle = true;
    unsigned long currTime;
    int msgCount = 0;
    unsigned long prevSend = 0;

    // meditationFlowCheckState
    bool meditationFlowCheck = false;

    //cabinLightsOn
    bool cabinLightOn = false;
    unsigned long cabinLightOnStartTime;

    // meditation
    unsigned long meditationStart;
    bool meditation = false;
    bool fidget = false;
    unsigned long fidgetStartTime;
    bool heartbeat = true;

    // end
    bool end = false;
    unsigned long endStart;


    // diffuser
    bool diffuserStart = false;
    unsigned long diffuserStartTime;
    unsigned long diffuserEndTime;
};
#endif
