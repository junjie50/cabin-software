#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLighting(){
  FastLED.clear(true);
}


// cabin lights turn on
void Light::cabinLightingSetup() {
  setColour(CRGB(LIGHTBLUE));
}

// cabin lights turn on
void Light::cabinLighting() {
  setColour(CRGB(LIGHTBLUE));
}

void Light::meditationFlowCheckLighting() {
  setColour(CRGB(LIGHTGREEN));
}

void Light::meditationLightingSetup() {
  setColour(CRGB(60,179,113));
}

void Light::meditationLighting() {
  return;
}
void Light::meditationNoHBLighting() {
  return;
}

// interval in miliseconds
void Light::glimmer(int interval) {
  const double half = interval / 2;
  unsigned long time = millis();
  double remainder = time % interval;
  if(remainder > half) {
    remainder = interval - remainder;
  }
  int brightness = remainder / (double)half * MAXBRIGHTNESS;
  setBrightness(brightness);
  update();
}


// Set the brightness of the LED
void Light::setBrightness(int brightness) {
  FastLED.setBrightness(brightness);
}

void Light::setColour(CRGB colour) {
  fill_solid(leds, NUMLEDS, colour);
}

void Light::clearLight() {
  FastLED.clear(true);
  update();
}

void Light::update() {
  FastLED.show();
}

void Light::setup() {
  FastLED.addLeds<WS2812, LEDPIN, GRB>(leds, NUMLEDS); 
  setBrightness(MAXBRIGHTNESS);
  clearLight();
  update();
}
