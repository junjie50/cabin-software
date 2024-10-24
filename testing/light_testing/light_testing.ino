#include "light.h"

Light lighting;

void setup() {
  // put your setup code here, to run once:
  lighting.setup();
  lighting.meditationFlowCheckLighting(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  lighting.meditationFlowCheckLighting(false);
}
