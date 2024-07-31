#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"
#include "Constants.h"


class Light {
  public:
    void idleLighting();
    void cabinLighting();
    void meditationFlowCheckLighting(bool start);

    void meditationLighting(int heartbeat);
    void meditationNoHBLighting();

    void endLighting();
    void lightUp();
    void lightFront();
    void lightMiddle();
    void lightBack();
    void lightAll(int power);
    void lightOff();
    void lightLow();
    void setup();
  private:
    int idleLight = 0;
    const int frontind = 0;
    const int leftind = 1;
    const int backind = 2;
    const int rightind = 3;
    int brightness[4] = {0, 0, 0, 0};
};
#endif
