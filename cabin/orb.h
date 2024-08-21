#ifndef orbClass_h
#define orbClass_h
#include <SoftwareSerial.h>
#include "Arduino.h"

class Orb {
  public:
    bool messageReady();
    void getMessage(char *buf);

    void setup();
    void sendMessage(char *msg);
    void receiveMessage(char *buf);
  private:
    const int RXPIN = 19;
    const int TXPIN = 18;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{RXPIN, TXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = true;
};

#endif
