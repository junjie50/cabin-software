#include "Arduino.h"
#define SECOND 1000

// Meditation flow check state
#define MEDITATIONFLOWCHECKSTATEGLIMMERINTERVAL 2000  // interval for orb to go from 0 to high to 0 brightness

// HEART RATE SENSOR ANALOG
#define HEARTRATESENSOR A1
#define BEATINTERVAL 300                // orb goes from 0 brightness to max to 0 in 0.3 seconds
#define BEATINTERVALHALF 150
#define VALIDHIGH 890                   // highest reading of hb sensor as valid
#define VALIDLOW 850                    // lowest reading of hb sensor as valid
#define HRTHRESHOLD 869                 // threshold for detection of hb
#define HRMININTERVAL 450               // wait 0.45 seconds before detecting again
#define RECALIBRATIONINTERVAL 30000

// MOTION SENSOR
#define MOTIONTHRESHOLD 3               // detect magnitude 3 at least 30 ms to send 1 motion signal
#define MOTIONDETECTDURATION 30         // detect at least 30ms to send 1 motion signal

// LED RING
#define LEDPIN 2
#define NUMLEDS 12
#define MAXBRIGHTNESS 40                // orb led maximum brightness

// BUZZER
#define BUZZERPIN 9
#define BUZZERINTERVAL 1000             // buzzer trigger after 1000 of no heart beat
#define BUZZERPOWER 200                 // range 80 - 255

// COLOURS FOR EACH STAGE
#define MEDITATIONFLOWLIGHT 0x3333ff    // colour for orb in meditation flow
#define MEDITATIONLIGHT 0x33ff33        // colour for orb in meditation
#define MEDITATIONLIGHTNOHB 0xff3333    // colour for orb when there is no hb in meditation