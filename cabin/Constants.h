#include <DFRobot_Heartrate.h>

#include "Arduino.h"
#define MINUTE 60000
#define SECOND 1000

// COMMUNICATION
#define REPEATMSG 3               //  How many times to repeat the msg sent to orb
#define MSGINTERVAL 300          //  intervals between the messages

// IDLE STATE
#define IDLELIGHTINTERVAL 4000    // How often does the light move from one place to another place.

//CABIN LIGHT ON STATE
#define CABINLIGHTINTERVAL 10     // 10 seconds no action in the cabin, cabin proceed to idle.
#define CHAIRDETECTTIME 3         // chair detected for 3 seconds, proceed to medi flow state.
#define CHAIRINTERVAL 1000        // The chair glimmers in 1 second interval.

// MEDITATION FLOW CHECK
#define CHAIRNOTDETECTTIME 10     // 10 seconds of chair no detection
#define HEARTBEATDETECTTIME 3     // 3 seconds of hcair no detection
#define MEDITATIONNODETECTTIME 5  // 5 second of detecting hr and chair pressure head to cabin lights on
#define FIDGETDETECTTIME 2        // 2 * 200 ms = 400ms of movement
#define FIDGETNOTDETECTTIME 50    // 50 * 200ms = 10000ms of no movement
#define TIMETODIM 4000            // 4 seconds to dim the room

// MEDITATION STATE
#define MEDITATIONDURATION 30000    //  Meditation state duration
#define FIDGETSPEAKERRATE 333       //  increasing fidget state volume of speaker every 333ms
#define FIDGETCHECKFREQUENCY 200    //  How often to check for the fidget
#define HEARTBEATLIGHTINTERVAL 350  // Cabin glimmer time for one heart beat
#define TIMEDIFFLIMIT 2000
#define AVGHEARTBEAT 60

// END STATE
#define ENDNOPRESSURE 10            // if 10 seconds of no one sitting, able to proceed to cabin light on
#define ENDPRESSUREDETECTED 60      // if user has been sitting for past 1 minute, continue with meditation
#define ENDLIGHTINTERVAL 1000       // cabin lights moves after 1 second
#define BASELIGHTINTERVAL 1000      // base glimmer at 1 second inter val
#define TIMETOLEAVE 10000           // time for user to leave the cabin before doing any logic such as checking chair pressure
#define BASECOLOUR 0x33ff33         // base colour


//SPEAKER CONSTANTS
#define SPEAKER2STARTVOL 20         // Fidget speaker start volume
#define SPEAKER2MAXVOLUME 30        // fidget speaker max volume
#define SPEAKERDEFAULTVOLUME 25     // default volume for both speaker

// PIR SENSOR
#define PIRPIN 53                   // Motion sensor pin

// BLUETOOTH RX TX
#define BTRXPIN 11
#define BTTXPIN 10

// PRESSURE SENSOR
#define PRESSUREPIN A15
#define PRESSUREHIGH 590            // Pressure sensor threshold, defines what is considered as sitting down

// LIGHT PWM PINS
#define NUMBERLIGHT 4           
#define LIGHTFRONT 45               // pin for front light (DOOR)
#define LIGHTLEFT 44                // pin to left light (inside cabin facing door)
#define LIGHTRIGHT 2                // pin for right light (inside cabin facing door)
#define LIGHTBACK 3                 // pin for back light
#define LIGHTCHAIR 4                // pin for chair light
#define MAXLIGHT 240                // maximum brightness for the cabin
#define MINLIGHT 1                  // minimum brightness for the cabin
#define LOWLIGHT 50                 // low light state
#define MAXLIGHTCHAIR 10            // maximum light for the chair

// DIFFUSER
#define DIFFUSERPIN 51
#define DIFFUSERDELAY 5000          // diffuser delay 5 seconds to trigger to improve user experience
#define DIFFUSERDELAYTRIGGER 5700   // after 5 seconds, trigger the diffuser for 0.7 seconds
#define DIFFUSERINTERVAL 700        // trigger high for 0.7 seconds to indicate a on/off

// BASE
#define LEDPIN 46     
#define NUMLEDS 12                  // number of LED in base
#define MAXBRIGHTNESS 40            // base maximum brightness

// Contact sensor
#define CONTACTPIN 47




