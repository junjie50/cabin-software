#include "sensor.h"

void Sensor:: setUp(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  Serial.println("MPU6050 Found!");

  //setup motion detectio off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(MOTIONTHRESHOLD);
  mpu.setMotionDetectionDuration(MOTIONDETECTDURATION);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn
  mpu.setMotionInterrupt(true);

  pinMode(BUZZERPIN,OUTPUT);    // Set the digital pin(9) as output
}

void Sensor::polling(Cabin cabin) {
  static unsigned long prevSecond = 0;
  static unsigned long prevHundred = 0;
  static unsigned long prevHB = 0;
  static bool prevValid = false;
  static unsigned long prevPoll = 0;

  unsigned long currTimeMilli = millis();
  // curr time in seconds
  unsigned long currTime = currTimeMilli / SECOND;
  unsigned long currTimeHundred= currTimeMilli / 100;

  // diff in time from the prev poll
  unsigned long diff = currTimeMilli- prevPoll;

  // PULSE TRACKER
  // Poll the heartbeat and store in pulse
  // pulse will change for every cycle
  pulse = heartBeatPoll();

  // keep a polling tag for pulse
  if(pulse) {
    prevValid = true;
    prevRecorded = currTime;
  }

  // keep track of prev heartbeat incase of no detection every 100ms
  unsigned long remainHB = millis() / 100;
  if(remainHB != prevHB) {
    prevHB = remainHB;
    if(prevValid) {
      noHBTime = 0;
      prevValid = false; // clear the polling tag for pulse
    }
    else {
      noHBTime++;
    }
  }

  // MOTION TRACKER
  // motion in this cycle default to false
  motion = false;
  // every second update from gyro
  if(currTimeHundred != prevHundred) {
    prevHundred = currTimeHundred;
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

  unsigned long diff = currTime - prevHeartBeat;
  if(diff > BUZZERINTERVAL) {
    unsigned long second = diff / 1000;
    if(second % 2 == 1) {
      analogWrite(BUZZERPIN, BUZZERPOWER);
    }
    else {
      analogWrite(BUZZERPIN, LOW);
    }
  }
  else {
    analogWrite(BUZZERPIN, LOW);
  }
}

bool Sensor::controlMoved() {
  return motion;
}

bool Sensor::heartBeat() {
  return pulse;
}

bool Sensor::heartBeatPoll() {
  static unsigned long detectTime = 0;
  static unsigned long startTime = 0;
  static bool pulse = false;
  static bool stable = false;
  static int highThreshold = 0;
  static unsigned int low = 2000;
  static unsigned int high = 0;
  static unsigned long monitorTime;
  static int validHigh = VALIDHIGH;
  static int validLow = VALIDLOW;
  static int calibrationCount = 0;

  unsigned long currTime = millis();
  int hbreading = analogRead(HEARTRATESENSOR);

  unsigned long timeDiff = currTime - detectTime;

  if(hbreading > validLow && hbreading < validHigh) { // valid reading
    if(timeDiff > RECALIBRATIONINTERVAL || firstTime) { // start of a new detection, set values for calibration
      low = 2000;
      high = 0;
      startTime = currTime;
      detectTime = currTime;
      stable = false;
      calibrationCount = 0;
      firstTime = false;
    }
    else {
      unsigned long timeDiffStart = currTime - startTime;
      if(timeDiffStart > 500) { // only allow after 1000ms to improve the timing
        if(!stable) { // calculate new threshold
          highThreshold = HRTHRESHOLD;
          stable = true;
        }
        else if(timeDiff > HRMININTERVAL && hbreading > highThreshold) { // not in pulse mode
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

bool Sensor::isCalibrating() {
  return calibrating;
}

bool Sensor::noHeartBeatFor(int duration) {
  return (noHBTime / 10) > duration;
}