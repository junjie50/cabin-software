#include "light.h"

// Each section lights up by turn and dimming
void Light::idleLighting() {
  static unsigned long idleLightInterval = 4000;
  static unsigned long idleLightIntervalHalf = 2000;
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
    currRemainder = idleLightInterval - currRemainder;
  }
  
  int output = (currRemainder / (double)idleLightIntervalHalf) * MAXLIGHT + MINLIGHT;
  brightness[idleLight] = output;
  lightUp();
  offChair();
}

// cabin lights turn on
void Light::cabinLighting() {
  static unsigned long intervalChair = 1000;
  static unsigned long intervalHalfChair = 500;
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
  static unsigned long interval = 4000;
  static unsigned long intervalHalf = 2000;

  static unsigned long intervalChair = 1000;
  static unsigned long intervalHalfChair = 500;
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
  static unsigned int interval = 250;
  static unsigned int half = 125;
  static unsigned long startTime = 0;
  static unsigned long TIMEDIFFLIMIT = 2000;
  static unsigned long defaultInterval = 60000/AVGHEARTBEAT;
  static unsigned long defaultIntervalHalf = defaultInterval / 2;
  

  unsigned currTime = millis();
  if(heartbeat) {
    startTime = currTime;
  }
  unsigned long timeDiff = currTime - startTime;
  if(timeDiff > interval) { // if not receiving heart beat for a long time.
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

void Light::lightChair(int power) {
  analogWrite(LIGHTCHAIR, power);
}

void Light::offChair() {
  analogWrite(LIGHTCHAIR, 0);
}

void Light::setup() {
  pinMode(LIGHTFRONT, OUTPUT);
  pinMode(LIGHTLEFT, OUTPUT);
  pinMode(LIGHTRIGHT, OUTPUT);
  pinMode(LIGHTBACK, OUTPUT);
}

