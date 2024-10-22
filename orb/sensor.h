#ifndef sensorClass_h
#define sensorClass_h
#include "Arduino.h"
#include "DFRobot_Heartrate.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Constants.h"
#include "cabin.h"

class Sensor {
  public:
    void setUp();
    void reset();
    void polling(Cabin cabin);
    bool isCalibrating();

    // HEART BEAT SENSOR OPERATIONS
    bool heartBeat();
    bool heartBeatPoll();
    bool noHeartBeatFor(int duration);

    // GYRO SENSOR DETECT MOVEMENT
    bool controlMoved();

    //BUZZER
    void triggerBuzzer(bool heartbeat);


  private:
    bool pulse = false;
    DFRobot_Heartrate heartrate{DIGITAL_MODE};
    Adafruit_MPU6050 mpu;
    
    unsigned long prevRecorded = 0;
    unsigned long noHBTime = 0;
    bool firstTime = true;
    bool calibrating = false;

    // acclerometer
    bool motion = false;
};
#endif