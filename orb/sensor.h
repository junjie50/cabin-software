#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "Constants.h"
#include "DFRobot_Heartrate.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "cabin.h"

class Sensor {
  public:
    void setUp();
    void reset();
    void polling(Cabin cabin);

    // HEART BEAT SENSOR OPERATIONS
    int getHeartBeat();
    bool heartBeat();
    bool noHeartBeatFor(int duration);

    // GYRO SENSOR DETECT MOVEMENT
    bool controlMoved();

    //BUZZER
    void triggerBuzzer(bool heartbeat);


  private:
    int heartbeat = 0;
    DFRobot_Heartrate heartrate{DIGITAL_MODE};
    Adafruit_MPU6050 mpu;
    bool pulse;
    
    unsigned long prevRecorded = 0;
    unsigned long noHBTime = 0;

    // acclerometer
    bool motion = false;
};
#endif