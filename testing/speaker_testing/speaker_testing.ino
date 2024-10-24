#include <DFRobot_DF1201S.h>
DFRobot_DF1201S player1;
DFRobot_DF1201S player2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  while(!player1.begin(Serial1)) {
    Serial.println("Init failed speaker1, please check the wire connection!");
    delay(1000);
  }
  while(!player2.begin(Serial2)) {
    Serial.println("Init failed speaker2, please check the wire connection!");
    delay(1000);
  }
  player1.setVol(20);  //Set volume value. From 0 to 30
  player2.setVol(20);  //Set volume value. From 0 to 30
  Serial.println(F("DFPlayer Mini online."));
  // enter music mode
  player1.switchFunction(player1.MUSIC);
  player2.switchFunction(player2.MUSIC);
  player1.setPlayMode(player1.SINGLE);
  player2.setPlayMode(player2.SINGLE);
  delay(2000);
}

void loop() {
  Serial.println("start");
  player1.playFileNum(1);
  delay(10000);
  Serial.println("stop");
  player1.pause();
  delay(10000);
  Serial.println("stop");
  player1.pause();
  delay(10000);
}
  