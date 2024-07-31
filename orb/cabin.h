#ifndef cabinClass_h
#define cabinClass_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Constants.h"

class Cabin {
    public:
    void setup();
    void sendMessage();
    void receiveMessage();
    
  private:
    const int RXPIN = 11;
    const int TXPIN = 12;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{RXPIN, TXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = true;
};
#endif
