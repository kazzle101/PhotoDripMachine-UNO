
unsigned int threshold = 0;     // current state of the phototransistor
unsigned int thresholdTol = 20;  // tolerance of the threshold

unsigned int flashWait = 340;   // time in millis between the laser being triggered and the flash being fired
unsigned int solenoidWait = 6;  // time in millis the solenoid is triggerd
unsigned int betweenDrips = 120;  // time in millis between drops of water - when doing more than one drip
unsigned long nowMillis;

unsigned long currentTime;
unsigned long loopTime;
const int timeout = 6000;
byte irAction = 0;

boolean complete = false;
boolean shutterOpen = false;
unsigned int val;              // reading of the analog port


struct OLEDTEXT {
  char line1[21] = {0};
  char line2[21] = {0};
  char line3[21] = {0};
  char line4[21] = {0};  
};
