#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"
#include "Constants.h"
#include <FastLED.h>

#define LIGHTGREEN 0x00cc99
#define LIGHTBLUE 0x00ff99


class Light {
  public:
    void idleLighting();

    void cabinLightingSetup();
    void cabinLighting();

    void meditationFlowCheckLighting();

    void meditationLightingSetup();
    void meditationLighting();

    void meditationNoHBLighting();
    void endLighting();

    void setBrightness(int brightness);
    void clearLight();
    void setColour(CRGB colour);
    void glimmer(int interval);
    void update();
    void setup();
  private:
    CRGB leds[NUMLEDS];
};
#endif
