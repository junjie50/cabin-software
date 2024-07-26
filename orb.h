#ifndef orbClass_h
#define orbClass_h
#include <SoftwareSerial.h>

class Orb {
  public:
    void setUp();
    void sendMessage(String data);
  private:
    const int RXPIN = 11;
    const int TXPIN = 12;
    char buffer[30];
    SoftwareSerial BTSerial;   // RX | TX
};

#endif
