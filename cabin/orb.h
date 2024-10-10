#ifndef orbClass_h
#define orbClass_h
#include <SoftwareSerial.h>
#include "Arduino.h"

class Orb {
  public:
    void setup();
    void sendMessage(char *msg);
    void receiveMessage();
    bool messageReady();
    void copyAndClearMessage(char *buffer);

  private:
    const int RXPIN = 10;
    const int TXPIN = 11;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{TXPIN, RXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = false;
};

#endif
