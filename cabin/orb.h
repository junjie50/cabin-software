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
    void receiveMessage();
    void copyAndClearMessage(char *buffer);

  private:
    const int RXPIN = 18;
    const int TXPIN = 19;
    char bufIn[15];
    char bufOut[15];
    SoftwareSerial BTSerial{TXPIN, RXPIN}; // RX | TX
    int inWriter = 0;
    int outWriter = 0;
    bool msgReady = false;
};

#endif
