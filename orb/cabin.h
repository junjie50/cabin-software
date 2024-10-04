#ifndef cabinClass_h
#define cabinClass_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <string.h>

class Cabin {
    public:
    void setup();
    void sendMessage(char *msg);
    void receiveMessage();
    bool messageReady();
    void copyAndClearMessage(char *buffer);
    
  private:
    const int RXPIN = 11;
    const int TXPIN = 10;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{TXPIN, RXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = false;
};
#endif
