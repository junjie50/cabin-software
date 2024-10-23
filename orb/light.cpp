#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLightingSetup(){
  FastLED.clear(true);
}


// cabin lights turn on
void Light::cabinLightingSetup() {
  FastLED.clear(true);
}

//Setup for meditation flow light
void Light::meditationFlowCheckLightingSetup() {
  setColour(MEDITATIONFLOWLIGHT);
}

void Light::meditationFlowCheckLighting(bool movement) {
  if(!movement) {
    glimmer(MEDITATIONFLOWCHECKSTATEGLIMMER);
  }
  else {
    setBrightness(MAXBRIGHTNESS);
    update();
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
  setColour(MEDITATIONLIGHT);
}

void Light::meditationLighting(bool heartbeat) {
  beatOnce(heartbeat);
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

// interval in 300ms
void Light::beatOnce(bool heartBeat) {
  static unsigned long startTime = 0;
  unsigned long time = millis();
  unsigned long diff = (time - startTime);
  if(heartBeat && diff > BEATINTERVAL) {
    setColour(MEDITATIONLIGHT);
    startTime = time;
  }

  int brightness;
  if(diff > BEATINTERVAL) {
    if(diff > 2000) {
      brightness = MAXBRIGHTNESS;
      setColour(MEDITATIONLIGHTNOHB);
    }
    else {
      brightness = 0;
    }
  }
  else if(diff > BEATINTERVALHALF) {
    double remainder = (double)(BEATINTERVAL - diff) / BEATINTERVALHALF ;
    brightness = remainder * MAXBRIGHTNESS;
  }
  else {
    double remainder = (double)diff / BEATINTERVALHALF ;
    brightness = remainder * MAXBRIGHTNESS;
  }
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
