#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLighting() {
  static unsigned long idleLightInterval = 2000;
  static unsigned long idleLightIntervalHalf = 1000;
  static unsigned long time = millis();
  static unsigned long prevRemainder = 0;
  time = millis();
  int currRemainder = time % idleLightInterval;
  if(prevRemainder > currRemainder) {
    brightness[idleLight] = 0;
    idleLight = (idleLight + 1) % 4;
  }
  prevRemainder = currRemainder;

  if(currRemainder > idleLightIntervalHalf) {
    currRemainder = idleLightIntervalHalf - currRemainder;
  }

  int output = (currRemainder / SECOND) * MAXLIGHT;
  brightness[idleLight] = output;
  lightUp();
}

// cabin lights turn on
void Light::cabinLighting() {
  lightAll(MAXLIGHT);
}

// meditation flow light sequence
void Light::meditationFlowCheckLighting(bool start) {
  static unsigned long startTime = 0;
  static unsigned long time;
  static unsigned long interval = 4000;
  if(start) {startTime = millis();} 

  time = millis();
  unsigned long remainder = (time - startTime);
  int power = (interval - remainder) / SECOND * MAXLIGHT;

  if(power < LOWLIGHT) {
    lightAll(LOWLIGHT);
  }
  else {
    lightAll(power);
  }
}

void Light::meditationLighting(int heartbeat) {
  static unsigned long time = millis();
  static unsigned minute = 60000;
  static unsigned int interval;
  if(heartbeat == 0) {
    heartbeat = AVGHEARTBEAT;
  }

  interval = minute / heartbeat;
  unsigned long half = interval / 2;
  time = millis();
  unsigned long remainder = time % interval;
  if(remainder > half) {
    remainder = (interval - remainder);
  }

  lightAll(remainder / SECOND * MAXLIGHT);
}

void Light::endLighting() {
  static unsigned long endLightInterval = 1000;
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

void Light::setup() {
  pinMode(LIGHTFRONT, OUTPUT);
  pinMode(LIGHTLEFT, OUTPUT);
  pinMode(LIGHTRIGHT, OUTPUT);
  pinMode(LIGHTBACK, OUTPUT);
}

