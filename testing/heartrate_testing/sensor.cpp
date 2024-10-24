#include "sensor.h"

void Sensor:: setUp(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setup motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(3);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  pinMode(BUZZERPIN,OUTPUT);    // Set the digital pin(9) as output
  delay(20);
}

void Sensor::polling() {
  static unsigned long prevSecond = 0;
  static unsigned long prevHB = 0;
  static unsigned long prevValid = 0;
  static unsigned long prevPoll = 0;

  unsigned long currTimeMilli = millis();
  // curr time in seconds
  unsigned long currTime = currTimeMilli / SECOND;

  // diff in time from the prev poll
  unsigned long diff = currTimeMilli- prevPoll;

  bool pulse = heartBeat();

  // keep track of prev heartbeat incase of no detection every 100ms
  unsigned long remainHB = millis() / 100;
  if(remainHB != prevHB) {
    prevHB = remainHB;
    if(prevValid) {
      noHBTime = 0;
      heartbeat = prevValid; // store the prevValid heartbeat
      prevValid = 0;
    }
    else {
      noHBTime++;
      if(noHBTime == 50) { // 5s without heartbeat clear heartbeat to 0
        heartbeat = 0;
      }
    }
  }

  // motion in this cycle default to false
  motion = false;
  // every second update from gyro
  if(currTime != prevSecond) {
    prevSecond = currTime;
    if(mpu.getMotionInterruptStatus()) {
      // if it is true, return true for this cycle
      motion = true;
    }
  }
}

void Sensor::reset() {
  analogWrite(BUZZERPIN, LOW);
  mpu.getMotionInterruptStatus();
}

void Sensor::triggerBuzzer(bool heartbeat){
  static unsigned long prevHeartBeat = 0;
  unsigned long currTime = millis();

  if(heartbeat) {
    prevHeartBeat = currTime;
  }

  int diff = currTime - prevHeartBeat;
  if(diff > 1000) {
    analogWrite(BUZZERPIN, BUZZERPOWER);
  }
  else{
    analogWrite(BUZZERPIN, LOW);
  }
}

bool Sensor::controlMoved() {
  return motion;
}

int Sensor::getHeartBeat() {
  return heartbeat;
}

bool Sensor::heartBeat() {
  static unsigned long detectTime = 0;
  static unsigned long startTime = 0;
  static bool pulse = false;
  static bool stable = false;
  static int highThreshold = 0;
  static unsigned int low = 2000;
  static unsigned int high = 0;
  static unsigned long monitorTime;
  static int validHigh = 870;
  static int validLow = 850;

  unsigned long currTime = millis();
  int hbreading = analogRead(HEARTRATESENSOR);

  unsigned long timeDiff = currTime - detectTime;
  if(hbreading > validLow && hbreading < validHigh) { // valid reading
    if(timeDiff > 2000) { // start of a new detection, set values for calibration
      low = 2000;
      high = 0;
      startTime = currTime;
      detectTime = currTime;
      stable = false;
    }
    else {
      unsigned long timeDiffStart = currTime - startTime;
      if(timeDiffStart > 500) { // only allow after 500ms to improve the timing
        if(timeDiffStart <= 1000) { // do the min and high calibration
          low = min(low, hbreading);
          high = max(high, hbreading);
        }
        else if(!stable) { // calculate new threshold
          highThreshold = (high + low) / 2 + 1;
          stable = true;
        }
        else if(timeDiff > 250 && hbreading > highThreshold) { // not in pulse mode
          detectTime = currTime;
          pulse = true;
        }
        else { // change pulse to false if prev was pulse or timeDiff < 300;
          pulse = false;
        } 
      }
    }
  }
  else { // not valid value
    pulse = false;
  } 

  return pulse;
}


bool Sensor::noHeartBeatFor(int duration) {
  unsigned long diff = millis() - prevRecorded;
  return diff > duration;
}