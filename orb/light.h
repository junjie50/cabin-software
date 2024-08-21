#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"
#include "Constants.h"
#include <FastLED.h>


class Light {
  public:
    void idleLightingSetup();
    void idleLighting();

    void cabinLightingSetup();
    void cabinLighting();

    void meditationFlowCheckLightingSetup();
    void meditationFlowCheckLighting(bool movement);

    void meditationLightingSetup();
    void meditationLighting(bool heartbeat);

    void endLightingSetup();
    void endLighting();

    void setBrightness(int brightness);
    void clearLight();
    void setColour(CRGB colour);
    void glimmer(int interval);
    void beatOnce(bool heartbeat);
    void update();
    void setup();
  private:
    CRGB leds[NUMLEDS];
};
#endif
