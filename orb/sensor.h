#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "Constants.h"
#include "DFRobot_Heartrate.h"
#include <Wire.h>

class Sensor {
  public:
    void setUp();
    void polling();

    // HEART BEAT SENSOR OPERATIONS
    int getHeartBeat();

    // GYRO SENSOR DETECT MOVEMENT
    bool controlMoved();

    //BUZZER
    void triggerBuzzer(int heartbeat);

  private:
    int heartbeat = 0;
    DFRobot_Heartrate heartrate{DIGITAL_MODE};

    // acclerometer
    const int MPU = 0x68; // MPU6050 I2C address
    float AccX, AccY, AccZ;
};
#endif