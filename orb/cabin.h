#ifndef cabinClass_h
#define cabinClass_h
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Constants.h"
#include <string.h>

class Cabin {
    public:
    void setup();
    void sendMessage(char *msg);
    void receiveMessage(char *buf);
    bool messageReady();
    void copyAndClearMessage(char *buffer);
    
  private:
    const int RXPIN = 0;
    const int TXPIN = 1;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{RXPIN, TXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = true;
};
#endif
