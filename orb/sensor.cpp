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
  mpu.setMotionDetectionDuration(30);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
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

  if(calibrating) {
    analogWrite(BUZZERPIN, LOW);
    return;
  }

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
  static int validHigh = 871;
  static int validLow = 850;
  static int calibrationCount = 0;

  unsigned long currTime = millis();
  int hbreading = analogRead(HEARTRATESENSOR);

  unsigned long timeDiff = currTime - detectTime;
  if(timeDiff < 2500) { // set the calibratng variable
    calibrating = true;
  }
  else {
    calibrating = false;
  }

  if(hbreading > validLow && hbreading < validHigh) { // valid reading
    if(timeDiff > 30000 || firstTime) { // start of a new detection, set values for calibration
      low = 2000;
      high = validHigh;
      startTime = currTime;
      detectTime = currTime;
      stable = false;
      calibrationCount = 0;
      firstTime = false;
    }
    else {
      unsigned long timeDiffStart = currTime - startTime;
      if(calibrationCount == 0) {
        Serial.println("calibrating");
        calibrationCount += 1;
      }
      if(timeDiffStart > 2000) { // only allow after 500ms to improve the timing
        if(timeDiffStart <= 2500) { // do the min and high calibration
          low = min(low, hbreading);
        }
        else if(!stable) { // calculate new threshold
          highThreshold = ((high + low)) / 2 + 1;
          stable = true;
        }
        else if(timeDiff > 450 && hbreading > highThreshold) { // not in pulse mode
          detectTime = currTime;
          Serial.println("b");
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