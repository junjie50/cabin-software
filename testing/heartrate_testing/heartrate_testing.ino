void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(1000);
}

bool start = false;
bool pulse = false;
unsigned long detectTime = 0;
unsigned long startTime = 0;
unsigned long detectionTime = 0;
bool stable = false;
int highThreshold = 0;
unsigned int low = 2000;
unsigned int high = 0;
unsigned int monitorTime;

// analog heartbeat detection
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currTime = millis();
  int hbreading = analogRead(A1);

  unsigned long timeDiff = currTime - detectTime;
  if(hbreading < 875 && hbreading > 850) { // valid reading
    if(timeDiff > 5000) { // start of a new detection, set values for calibration
      low = 2000;
      high = 0;
      startTime = currTime;
      detectTime = currTime;
      stable = false;
    }
    else{
      unsigned long timeDiffStart = currTime - startTime;
      if(timeDiffStart <= 2500) {
        Serial.println("Waiting for calibration");
      }
      else if(timeDiffStart > 2500 && timeDiffStart < 3000) { // do the min and high calibration
        Serial.println("calibrating");
        low = min(low, hbreading);
        high = max(high, hbreading);
      }
      else if(!stable) { // calculate new threshold
        highThreshold = (high + low) / 2 + 1;
        stable = true;
      }
      else if(timeDiff > 400 && hbreading > highThreshold) { // not in pulse mode
        detectTime = currTime;
        pulse = true;
      }
      else { // change pulse to false if prev was pulse or timeDiff < 300;
        pulse = false;
      }
    }
  }
  else { // not valid value
    pulse = false;
  }

  if(pulse){
    Serial.println("pulse");
  }
}
