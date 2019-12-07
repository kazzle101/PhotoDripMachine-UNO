#include <IRremote.h>
#include <Wire.h>
#include "PCF8574.h" // https://skyduino.wordpress.com/librairie-arduino-pcf8574-pcf8575/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

PCF8574 pcf0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     3 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define IR_RECV_PIN 12
IRrecv irrecv(IR_RECV_PIN);
decode_results IRresult;
boolean irPWR = false;
boolean keepLampOff = false;

#define LED_A 6    // yellow
#define LED_B 7    // red

// digital IO
//   O  O  O  O  O  O
//   5V D3 D4 D5 D6 GND
#define CAMERA_FLASH 8
#define CAMERA_SHOOT 7
#define MAINS_SW 2
#define SOLENOID 4
#define LASER 6

// analog inputs
//   O  O  O  O  O  O
//  GND A0 A1 A2 A3 5V
#define PHOTO A1


#include "configuration.h"
OLEDTEXT oledText;

#include "oled.h"
#include "shutter.h"
#include "changesettings.h"
#include "waterdrops.h"


void setup() {

  delay (1000);
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  strcat(oledText.line1, "setup");
  oledTextOutput();
  
  pcf0.begin(0x38);
  pcf0.pinMode(LED_A, OUTPUT);  // red
  pcf0.pinMode(LED_B, OUTPUT);  // yellow   also TRIGGER_LED

  pinMode(CAMERA_FLASH, OUTPUT);
  pinMode(CAMERA_SHOOT, OUTPUT);
  pinMode(MAINS_SW, OUTPUT);
  pinMode(SOLENOID, OUTPUT);
  pinMode(LASER, OUTPUT);

  digitalWrite(CAMERA_FLASH, LOW);
  digitalWrite(CAMERA_SHOOT, LOW);
  digitalWrite(MAINS_SW, HIGH);
  pcf0.digitalWrite(LED_A, HIGH); // yellow
  pcf0.digitalWrite(LED_B, LOW);  // red
  digitalWrite(SOLENOID, LOW);
  digitalWrite(LASER, HIGH);

  irrecv.enableIRIn(); // Start the IR receiver
  currentTime = millis();
  loopTime = currentTime;
  
  oledShowMainOptions();
  delay(500);

}


void loop() {

  currentTime = millis();

  if (currentTime >= (loopTime + 5)) {
    if (irrecv.decode(&IRresult)) {
      // Serial.println(IRresult.value, HEX);
      switch (IRresult.value) {
        case (0xC0E8):   // Power
          irPWR = true;
          break;
        case (0xC091):   // Play
          irPWR = true;
          break;
        case (0xC061):   // Stop
          irAction = 0;
          irPWR = false;
          closeShutter();
          shutterOpen = false;
          break;

//        case (0xC05E):    // menu
//          irAction = 0;
//          irPWR = false;
//          closeShutter();
//          shutterOpen = false;
//          changeSettings();
//          break;

        case (0xC084):    // 001
          irPWR = true;
          irAction = 1;  // single drop of water
          break;
        case (0xC044):    // 002
          irPWR = true;
          irAction = 2;  // two drops of water
          break;
        case (0xC0C4):    // 003
          irPWR = true;
          irAction = 3;  // three drops of water
          break;

        case (0xC078):    // Volume Up
          changeFlashWait(10);
          break;
        case (0xC0F8):    // Volume Down
          changeFlashWait(-10);
          break;
        case (0xC098):    // Channel Up
          changeFlashWait(5);
          break;
        case (0xC018):    // Channel Down
          changeFlashWait(-5);
          break;
        case (0xC07D):    //  Fast Forward
          changeFlashWait(2);
          break;
        case (0xC0FD):    //  Fast Reverse
          changeFlashWait(-2);
          break;
        case (0xC0A4):    // 005
          changeFlashWait(1);
          break;
        case (0xC014):    // 008
          changeFlashWait(-1);
          break;

        case (0xC03E):  // arrow up
          changeLaserTolerance(2);
          break;

        case (0xC0DE):  // arrow down
           changeLaserTolerance(-2);
          break;      
        
        case (0xC024):    // 004
          changeBetweenDrips(1);
          break;
        case (0xC0E4):    // 007
          changeBetweenDrips(-1);
          break;

        //      case (0xC024):    // 004
        //        stringStatus(4, false, irPWR);
        //        break;
        //      case (0xC0A4):    // 005
        //        stringStatus(5, false, irPWR);
        //        break;
        //      case (0xC064):    // 006
        //        stringStatus(6, false, irPWR);
        //        break;
        //      case (0xC0E4):    // 007
        //        stringStatus(7, false, irPWR);
        //        break;
        //      case (0xC014):    // 008
        //        stringStatus(8, false, irPWR);
        //        break;

        case (0xC094):    // 009
          irAction = 0;
          solenoidOpen(9000);
          break;

        case (0xC004):    // 000
          irAction = 0;
          dripSequence();
          break;
      }
      irrecv.resume(); // Receive the next value
    }
    loopTime = currentTime;
  }

  if (irPWR == true && irAction >= 1) {
    multipleDropsOfWater(irAction);
    irPWR = false;
  }
}
