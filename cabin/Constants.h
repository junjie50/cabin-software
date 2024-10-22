#include <DFRobot_Heartrate.h>

#include "Arduino.h"
#define SECOND 1000

//CABIN LIGHT ON 
#define CABINLIGHTINTERVAL 10
#define CHAIRDETECTTIME 3


#define CHAIRNOTDETECTTIME 10
#define HEARTBEATDETECTTIME 3
#define MEDITATIONNODETECTTIME 5
#define FIDGETDETECTTIME 2

#define SPEAKERLOOPCOUNT 5
#define SPEAKERLOOPCOUNTMINUSONE 4

#define ENDNOPRESSURE 10
#define ENDPRESSUREDETECTED 60



//SPEAKER CONSTANTS
#define SPEAKER2STARTVOL 20
#define SPEAKER2MAXVOLUME 30

// PIR SENSOR
#define PIRPIN 53

// BLUETOOTH RX TX
#define BTRXPIN 11
#define BTTXPIN 10

// PRESSURE SENSOR
#define PRESSUREPIN A15
#define PRESSUREHIGH 590

// LIGHT PWM PINS
#define LIGHTFRONT 45
#define LIGHTLEFT 44
#define LIGHTRIGHT 2
#define LIGHTBACK 3
#define LIGHTCHAIR 4

#define MAXLIGHT 240
#define MINLIGHT 1
#define LOWLIGHT 50

#define MAXLIGHTCHAIR 10

// HEARTBEAT 
#define AVGHEARTBEAT 60

// DIFFUSER PIN
#define DIFFUSERPIN 51
#define DIFFUSERDELAY 5000
#define DIFFUSERDELAYTRIGGER 6000


// LED RING
#define LEDPIN 46
#define NUMLEDS 12
#define MAXBRIGHTNESS 40

// Contact sensor
#define CONTACTPIN 47




