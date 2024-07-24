#ifndef speakerClass_h
#define speakerClass_h

class Speaker {
  public:
    void speakerReset();
    void meditationSpeaker(); // returns the number of times a song looped
    void endStateSpeaker(); // returns the number of times a song looped
    int getLoopedTimes();
  private:
};

#endif
