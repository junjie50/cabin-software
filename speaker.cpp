#include "speaker.h"

void Speaker::speakerSetUP() {
  SPEAKER1SERIAL.begin(152000);
  SPEAKER2SERIAL.begin(152000);
  while(!player1.begin(SPEAKER1SERIAL)) {
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  while(!player2.begin(SPEAKER2SERIAL)) {
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
}

void Speaker::speakerReset() {
  loopedCount1 = -1;
}

void Speaker::speaker1Stop() {
  player1.pause();
}
void Speaker::speaker2Stop() {
  player2.pause();
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
}

void Speaker::fidgetStateSpeakerSetUp() {
  player2Vol = SPEAKER2STARTVOL;
  player2.setVol(SPEAKER2STARTVOL);
}

void Speaker::fidgetStateSpeaker() {
  unsigned long currTime = millis();
  int remainder = (currTime / SECOND) % 5;
  if(!player2.isPlaying()) {
     player2.playFileNum(1);
  }
  else if(remainder == 0) {
    player2Vol++;
    player2.setVol(player2Vol);
  }
}

