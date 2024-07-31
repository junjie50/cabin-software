#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLightingSetup(){
  FastLED.clear(true);
}


// cabin lights turn on
void Light::cabinLightingSetup() {
  FastLED.clear(true);
}


// light blue rgb(173, 216, 230)
void Light::meditationFlowCheckLightingSetup() {
  setColour(LIGHTBLUE);
}

void Light::meditationFlowCheckLighting(int movement) {
  if(!movement) {
    glimmer(MEDITATIONFLOWCHECKSTATEGLIMMER);
  }
}

/*
################################
||                            ||
||         meditation         ||
||                            ||
################################
*/
// green light
void Light::meditationLightingSetup() {
  setColour(LIGHTGREEN);
}

void Light::meditationLighting(int heartbeat) {
  if(heartbeat == 0) {
    // soft pink
    setColour(SOFTPINK);
    setBrightness(MAXBRIGHTNESS);
    update();
  }
  else{
    glimmer(60000/heartbeat);
  }
}

void Light::endLightingSetup() {
   FastLED.clear(true);
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
