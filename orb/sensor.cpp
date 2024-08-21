#include "sensor.h"

void Sensor:: setUp(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(3);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  pinMode(BUZZERPIN,OUTPUT);    // Set the digital pin(11) as output
  delay(20);
}

void Sensor::polling(Cabin cabin) {
  static unsigned long prevSecond = 0;
  static unsigned long prevHB = 0;
  static unsigned long prevValid = 0;
  static unsigned long prevPoll = 0;
  unsigned long currTimeMilli = millis();
  unsigned long diff = currTimeMilli- prevPoll;
  unsigned long currTime = currTimeMilli / SECOND;
  int currHeartBeat = 0;

  // only poll hr sensor everyone 3ms
  if(diff > 30) {
    prevPoll = currTimeMilli;
    heartrate.getValue(HEARTRATESENSOR);   // A1 foot sampled values
    currHeartBeat = heartrate.getRate();   // A1 foot sampled values
  }

  // keep recording prev heartrate
  if(currHeartBeat) {
    prevValid = currHeartBeat;
    prevRecorded = currTimeMilli;
    pulse = true;
    Serial.println("pulse true");
  }
  else{
    pulse = false;
  }

  unsigned long remainHB = millis() / 100;
  if(remainHB != prevHB) {
    prevHB = remainHB;
    if(prevValid) {
      noHBTime = 0;
      heartbeat = prevValid;
      prevValid = 0;
    }
    else {
      noHBTime++;
      if(noHBTime == 50) { // 2 s without heartbeat clear heartbeat to 0
        heartbeat = 0;
      }
    }
  }

  // every second update from gyro
  if(currTime != prevSecond) {
    prevSecond = currTime;
    if(mpu.getMotionInterruptStatus()) {
      motion = true;
    }
    else {
      motion = false;
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
  return pulse;
}

bool Sensor::noHeartBeatFor(int duration) {
  unsigned long diff = millis() - prevRecorded;
  return diff > duration;
}

