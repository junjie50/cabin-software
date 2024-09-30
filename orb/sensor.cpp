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

  pinMode(BUZZERPIN,OUTPUT);    // Set the digital pin(9) as output
  delay(20);
}

void Sensor::polling(Cabin cabin) {
  static unsigned long prevSecond = 0;
  static unsigned long prevHB = 0;
  static unsigned long prevValid = 0;
  static unsigned long prevPoll = 0;

  unsigned long currTimeMilli = millis();
  // curr time in seconds
  unsigned long currTime = currTimeMilli / SECOND;

  // diff in time from the prev poll
  unsigned long diff = currTimeMilli- prevPoll;


  int currHeartBeat = 0;

  // Keep collecting the data, if diff is greater than 30, check the sensors for next pulse
  if(diff > 30) {
    heartrate.getValue(HEARTRATESENSOR);
    currHeartBeat = heartrate.getRate();   // A1 foot sampled values
  }

  // keep recording prev heartrate, if currHeartBeat > 0, pulse is true
  if(currHeartBeat) {
    // only poll hr sensor every 30ms, 120 per minute is 50ms interval
    prevPoll = currTimeMilli;
    prevValid = currHeartBeat;
    prevRecorded = currTimeMilli;
    pulse = true;
    Serial.println("pulse true");
  }
  else{ // reset pulse to false.
    pulse = false;
  }


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
      if(noHBTime == 50) { // 5 s without heartbeat clear heartbeat to 0
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
  return pulse;
}

bool Sensor::noHeartBeatFor(int duration) {
  unsigned long diff = millis() - prevRecorded;
  return diff > duration;
}

