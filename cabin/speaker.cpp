#include "speaker.h"

void Speaker::speakerSetUP() {
  SPEAKER1SERIAL.begin(115200);
  SPEAKER2SERIAL.begin(115200);
  while(!player1.begin(SPEAKER1SERIAL)) {
    Serial.println("Init failed speaker1, please check the wire connection!");
    delay(1000);
  }
  while(!player2.begin(SPEAKER2SERIAL)) {
    Serial.println("Init failed speaker2, please check the wire connection!");
    delay(1000);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  player1.setVol(5);  //Set volume value. From 0 to 30
  player2.setVol(5);  //Set volume value. From 0 to 30
  player1.start();
}

void Speaker::speakerReset() {
  loopedCount1 = 0;
}

void Speaker::speaker1Stop() {
  if(player1.isPlaying()){
    player1.pause();
  }
}

void Speaker::speaker2Stop() {
  if(player2.isPlaying()) {
    player2.pause();
  }
}

// after count it wil; not auto player
void Speaker::meditationSpeaker() {
  if(!player1.isPlaying()){
    loopedCount1++;
    player1.playFileNum(1);
  }
}

int Speaker::getLoopedTimes() {
  if(player1.isPlaying()) {
    return loopedCount1 - 1;
  }
  else {
    return loopedCount1;
  }
}

void Speaker::fidgetStateSpeakerSetUp() {
  player2Vol = SPEAKER2STARTVOL;
  player2.setVol(SPEAKER2STARTVOL);
}

void Speaker::fidgetStateSpeaker(unsigned long startTime) {
  static int prevVolume = SPEAKER2STARTVOL;
  unsigned long currTime = millis();
  int volume = min(SPEAKER2MAXVOLUME, SPEAKER2STARTVOL + (startTime - currTime) / SECOND  / 5);
  if(!player2.isPlaying()) {
     player2.playFileNum(1);
  }
  else if(prevVolume != volume){
    prevVolume = volume;
    player2.setVol(volume);
  }
}

void Speaker::endStateSpeaker() {
  if(!player1.isPlaying()){
    loopedCount1++;
    player1.playFileNum(1);
  }
}