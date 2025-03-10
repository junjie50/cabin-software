#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLighting() {
  static unsigned long idleLightInterval = IDLELIGHTINTERVAL;
  static unsigned long idleLightIntervalHalf = idleLightInterval / 2;
  static unsigned long time = millis();
  static unsigned long prevRemainder = 0;
  time = millis();
  int currRemainder = time % idleLightInterval;

  if(prevRemainder > currRemainder) {
    brightness[idleLight] = 0;
    idleLight = (idleLight + 1) % NUMBERLIGHT;
  }
  prevRemainder = currRemainder;

  if(currRemainder > idleLightIntervalHalf) {
    currRemainder = idleLightInterval - currRemainder;
  }
  
  int output = (currRemainder / (double)idleLightIntervalHalf) * MAXLIGHT + MINLIGHT;
  brightness[idleLight] = output;
  lightUp();
  offChair();
}

// cabin lights turn on
void Light::cabinLighting() {
  static unsigned long intervalChair = CHAIRINTERVAL;
  static unsigned long intervalHalfChair = intervalChair / 2;
  static unsigned long time;
  time = millis();
  lightAll(MAXLIGHT);

  unsigned long remainder = time % intervalChair;
  if(remainder > intervalHalfChair) {
    remainder = intervalChair - remainder;
  }

  int powerChair = (remainder / (double)intervalHalfChair) * MAXLIGHTCHAIR ;
  lightChair(powerChair);
}

// meditation flow light sequence
void Light::meditationFlowCheckLighting(bool start) {
  static unsigned long startTime = 0;
  static unsigned long time;
  static unsigned long interval = TIMETODIM;
  static unsigned long intervalHalf = interval / 2;

  if(start) {startTime = millis();} 

  time = millis();
  long timeDiff = (time - startTime);
  int power = ((long)interval - timeDiff) / (double)interval * MAXLIGHT + MINLIGHT;

  if(power < MINLIGHT) {
    lightAll(MINLIGHT);
  }
  else {
    lightAll(power);
  }
  offChair();
}

void Light::meditationLighting(int heartbeat) {
  static unsigned long time = millis();
  static unsigned int interval = HEARTBEATLIGHTINTERVAL;
  static unsigned int half = interval / 2;
  static unsigned long startTime = 0;
  static unsigned long defaultInterval = MINUTE/AVGHEARTBEAT;
  static unsigned long defaultIntervalHalf = defaultInterval / 2;
  

  unsigned currTime = millis();
  if(heartbeat) {
    startTime = currTime;
  }
  unsigned long timeDiff = currTime - startTime;
  if(timeDiff > interval) { 
    if(timeDiff > TIMEDIFFLIMIT) { // if never receive for too long, go to default human heart beat.
      int remainder = timeDiff % defaultInterval;
      if(remainder > interval) {
        return;
      }

      if(remainder > half) {
        remainder = interval - remainder;
      }

      int power = (remainder / (double) half) * MAXLIGHT + MINLIGHT;
      lightAll(power);
    }
    else{ // if within heartbeat range, just lightAll to low
      lightAll(MINLIGHT);
    }
    return;
  }

  if(timeDiff > half) {
    timeDiff = (interval - timeDiff);
  }
  int power = (timeDiff / (double)half) * MAXLIGHT + MINLIGHT;
  lightAll(power);
}

void Light::endLighting() {
  static unsigned long endLightInterval = ENDLIGHTINTERVAL;
  static unsigned long time;
  static unsigned long prevRemainder = 0;

  time = millis();
  int currRemainder = time % endLightInterval;

  if(prevRemainder > currRemainder) {
    idleLight = (idleLight + 1) % 3;
  }

  if(idleLight == 0) {
    lightBack();
  }
  else if(idleLight == 1) {
    lightMiddle();
  }
  else {
    lightFront();
  }
  prevRemainder = currRemainder;
}


void Light::lightUp() {
  analogWrite(LIGHTFRONT, brightness[0]);
  analogWrite(LIGHTLEFT, brightness[1]);
  analogWrite(LIGHTRIGHT, brightness[3]);
  analogWrite(LIGHTBACK, brightness[2]);
}

void Light::lightFront() {
  analogWrite(LIGHTFRONT, MAXLIGHT);
  analogWrite(LIGHTLEFT, 0);
  analogWrite(LIGHTRIGHT, 0);
  analogWrite(LIGHTBACK, 0);
}

void Light::lightMiddle() {
  analogWrite(LIGHTFRONT, 0);
  analogWrite(LIGHTLEFT, MAXLIGHT);
  analogWrite(LIGHTRIGHT, MAXLIGHT);
  analogWrite(LIGHTBACK, 0);
}

void Light::lightBack() {
  analogWrite(LIGHTFRONT, 0);
  analogWrite(LIGHTLEFT, 0);
  analogWrite(LIGHTRIGHT, 0);
  analogWrite(LIGHTBACK, MAXLIGHT);
}

void Light::lightLow() {
  analogWrite(LIGHTFRONT, LOWLIGHT);
  analogWrite(LIGHTLEFT, LOWLIGHT);
  analogWrite(LIGHTRIGHT, LOWLIGHT);
  analogWrite(LIGHTBACK, LOWLIGHT);
}

void Light::lightAll(int power) {
  analogWrite(LIGHTFRONT, power);
  analogWrite(LIGHTLEFT, power);
  analogWrite(LIGHTRIGHT, power);
  analogWrite(LIGHTBACK, power);
}

void Light::lightOff() {
  analogWrite(LIGHTFRONT, 0);
  analogWrite(LIGHTLEFT, 0);
  analogWrite(LIGHTRIGHT, 0);
  analogWrite(LIGHTBACK, 0);
}

void Light::lightChair(int power) {
  analogWrite(LIGHTCHAIR, power);
}

void Light::offChair() {
  analogWrite(LIGHTCHAIR, 0);
}

void Light::baseLighting(bool item) {
  static unsigned int interval = BASELIGHTINTERVAL;
  static unsigned int half = interval / 2;
  setColour(BASECOLOUR);
  if(!item) {
    unsigned long time = millis();
    double remainder = time % interval;
    if(remainder > half) {
      remainder = interval - remainder;
    }
    int brightness = remainder / (double)half * MAXBRIGHTNESS;
    setBrightness(brightness);
  }
  else {
    setBrightness(0);
  }
  update();
}

void Light::baseOff() {
  clearLight();
}

// Set the brightness of the LED
void Light::setBrightness(int brightness) {
  FastLED.setBrightness(brightness);
}

void Light::clearLight() {
  FastLED.clear(true);
}

void Light::setColour(CRGB colour) {
  fill_solid(leds, NUMLEDS, colour);
}

void Light::update() {
  FastLED.show();
}

void Light::setup() {
  pinMode(LIGHTFRONT, OUTPUT);
  pinMode(LIGHTLEFT, OUTPUT);
  pinMode(LIGHTRIGHT, OUTPUT);
  pinMode(LIGHTBACK, OUTPUT);

  FastLED.addLeds<WS2812, LEDPIN, GRB>(leds, NUMLEDS);
  setColour(BASECOLOUR);
  clearLight();
  update();
}


