#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"

class Light {
  public:
    void idleLighting();
    void cabinLighting();
    void meditationFlowCheckLighting();

    void meditationLighting();
    void meditationNoHBLighting();

    void endLighting();
  private:
};
#endif
