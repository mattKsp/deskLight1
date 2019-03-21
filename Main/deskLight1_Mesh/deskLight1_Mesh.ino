/*
    'deskLight1_Mesh' by Thurstan. LED strip desk light.
    Copyright (C) 2019  MTS Standish (mattThurstan)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be usefull,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    https://github.com/mattThurstan/
*/


/*----------------------------libraries----------------------------*/
#include <EEPROM.h>                           // a few saved settings
//#include <Bounce2.h>                          // buttons with de-bounce
#include <FastLED.h>                          // WS2812B LED strip control and effects
//#include <Wire.h>                             // include, but do not need to initialise - for DS3231 & CAP1296
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version
#include "painlessMesh.h"
#include <MT_LightControlDefines.h>

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_A";
const String _progVers = "0.28";              // moved to ESP8266 mesh network
#define DEBUG 1                               // 0 or 1
boolean _debugOverlay = false;                // show debug overlay (eg. show segment endpoints)
boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              // this should init false, then get activated by input - on/off true/false
bool shouldSaveSettings = false; // flag for saving data
bool runonce = true; // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;                 // just in case  - using FastLED.delay instead..

/*----------------------------pins----------------------------*/
//const int _ledPin = 13;                       // built-in LED
const int _ledDOutPin = 14;                   // DOut 0 -> LED strip 0 DIn
//const int _i2cSDApin = 4;                     // SDA (D2)
//const int _i2cSCLpin = 5;                     // SCL (D1)

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 7;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7, 8 }; // test basic, tap bt to cycle around a few mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                    // current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                   // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
String modeName[_modeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };

/*----------------------------buttons----------------------------*/
//const unsigned long _buttonDebounceTime = 5;  // unsigned long (5ms)
//Bounce _button0 = Bounce();                   // Instantiate a Bounce object
//Bounce _button1 = Bounce();                   // Instantiate a Bounce object
//boolean _button0Toggled = false;
//boolean _button1Toggled = false;

/*----------------------------touch sensors----------------------------*/
Mpr121 mpr121;                                // init MPR121 on I2C
u16 touch_status_flag[CHANNEL_NUM] = { 0 };   // u16 = unsigned short

/*----------------------------LED----------------------------*/
#define MAX_POWER_DRAW 2850                   // limit power draw to 2.85A at 5v (with 3A power supply this gives us a bit of head room for board, lights etc.)
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNum = 150;                      // 5m strip with 150 LEDs
const int _segmentTotal = 4;                  // more later..
const int _ledGlobalBrightness = 255;         // global brightness
int _ledGlobalBrightnessCur = 255;            // current global brightness - adjust this
int _ledBrightnessIncDecAmount = 10;          // the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay  //100
LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 11, 12 }, 
  { 12, 46, 35 }, 
  { 47, 59, 12 },
  { 60, 94, 34 }
};                     
CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );
CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );

CRGB leds[_ledNum];                           // global RGB array
int _ledState = LOW;                          // use to toggle LOW/HIGH (ledState = !ledState)
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                        // current colour temperature

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;
String _modeString = "Glow";
uint32_t id = DEVICE_ID_BRIDGE1;

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG) { Serial.printf("deskLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (runonce == true) {
    publishState(false);
    publishBrightness(false);
    //publishRGB(false);
    //publishMode(false);
    runonce = false;
  }
  if (DEBUG) { Serial.printf("--> deskLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  if (DEBUG) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}


/*----------------------------MAIN----------------------------*/
void setup() {
  
  Serial.begin(115200);
  
  if (DEBUG) {
    Serial.println();
    Serial.print(_progName);
    Serial.print(" v");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  }
  
  delay(3000);                                // give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();
  setupMesh();
  
  if (DEBUG) {
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println("-----");
    Serial.print(F("Device Node ID is "));
    String s = String(mesh.getNodeId());
    Serial.println(s);
    Serial.println("-----");
    Serial.println("");
  }
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    if (DEBUG) { }
    _firstTimeSetupDone = true;               // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  mesh.update();
  loopUserInputs();
  loopModes();
  
  if (_debugOverlay) {
    checkSegmentEndpoints();
    //showColorTempPx();
  }
  
  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);                      // using FastLED.delay instead..
}


/*----------------------------Serial----------------------------*/
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//#ifdef DEBUG
//void serialEvent() {
//  while (Serial.available()) {
//    char inChar = (char)Serial.read();        // get the new byte:
//    _inputString += inChar;                   // add it to the inputString:
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}
//#endif

