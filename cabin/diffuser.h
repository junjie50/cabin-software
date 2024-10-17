#ifndef diffuserClass_h
#define diffuserClass_h
#include "Arduino.h"
#include "Constants.h"

class Diffuser {
  public:
    void setup();
    void trigger(unsigned long startTime, bool start);
  private:
};

#endif
