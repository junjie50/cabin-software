#include "sensor.h"

void Sensor:: setUp(){
  Wire.begin();
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission

  pinMode(BUZZERPIN,OUTPUT);    // Set the digital pin(11) as output
  delay(20);
}

void Sensor::polling() {
  static unsigned long prevSecond = 0;
  unsigned long currTime = millis() / SECOND;
  if(currTime != prevSecond) {
    prevSecond = currTime;
    heartbeat = heartrate.getValue(HEARTRATESENSOR);

    // get gyro accelerations
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value
  }
}

void Sensor::triggerBuzzer(int heartbeat){
  if(heartbeat > 0) {
    digitalWrite(BUZZERPIN, LOW);
  }
  else{
    const int remainder = millis() % BUZZERINTERVAL;
    const int half = BUZZERINTERVAL / 2;
    if(remainder > half) {
      digitalWrite(BUZZERPIN, LOW);
    }
    else{
      digitalWrite(BUZZERPIN, HIGH);
    }
  }
}

bool Sensor::controlMoved() {
  return false;
}

int Sensor::getHeartBeat() {
  return heartbeat;
}

