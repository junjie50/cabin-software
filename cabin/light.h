#ifndef lightClass_h
#define lightClass_h
#include "Arduino.h"
#include "Constants.h"
#include <FastLED.h>


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
    void lightChair(int power);
    void offChair();
    void setup();

    // base control
    void baseLighting(bool item);
    // Set the brightness of the LED
    void setBrightness(int brightness);
    void clearLight();
    void setColour(CRGB colour);
    void update();
    
  private:
    int idleLight = 0;
    const int frontind = 0;
    const int leftind = 1;
    const int backind = 2;
    const int rightind = 3;
    int brightness[4] = {0, 0, 0, 0};

    CRGB leds[NUMLEDS];
};
#endif
