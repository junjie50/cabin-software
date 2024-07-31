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
    void meditationFlowCheckLighting(int movement);

    void meditationLightingSetup();
    void meditationLighting(int heartbeat);

    void endLightingSetup();
    void endLighting();

    void setBrightness(int brightness);
    void clearLight();
    void setColour(CRGB colour);
    void glimmer(int interval);
    void update();
    void setup();
  private:
    CRGB leds[NUMLEDS];
    CRGB SOFTPINK = CRGB(244,194,194);
    CRGB LIGHTGREEN = CRGB(218,247,166);
    CRGB LIGHTBLUE = CRGB(173,216,230);
};
#endif
