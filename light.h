#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"
#include "Constants.h"

class Light {
  public:
    void idleLighting();
    void cabinLighting();
    void meditationFlowCheckLighting(bool start);

    void meditationLighting();
    void meditationNoHBLighting();

    void endLighting();
    void lightUp();
    void lightAll(int power);
    void lightOff();
    void lightLow();
    void setup();
  private:
    unsigned long idleLightInterval = 2000;
    unsigned long idleLightIntervalHalf = 1000;
    int idleLight = 0;
    const int frontind = 0;
    const int leftind = 1;
    const int backind = 2;
    const int rightind = 3;
    int brightness[4] = {0, 0, 0, 0};
};
#endif
