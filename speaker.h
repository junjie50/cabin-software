#ifndef speakerClass_h
#define speakerClass_h
#include <DFRobot_DF1201S.h>
#include "SoftwareSerial.h"
#include "Constants.h"
//DFPlayer Pro
class Speaker {
  public:
    void speakerSetUP();
    void speakerReset();
    void speaker1Stop();
    void speaker2Stop();
    void meditationSpeaker(); // returns the number of times a song looped
    void endStateSpeaker(); // returns the number of times a song looped
    void fidgetStateSpeakerSetUp(); // returns the number of times a song looped
    void fidgetStateSpeaker(); // returns the number of times a song looped
    int getLoopedTimes();
  private:
    DFRobot_DF1201S player1;
    DFRobot_DF1201S player2;
    int loopedCount1 = 0;
    int loopedCount2 = 0;
    int player2Vol = 10;
};

#endif
