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
// board running at [ 80mhz || this-> 1600mhz ]
#include <EEPROM.h>                           // a few saved settings
//#define FASTLED_ESP8266_D1_PIN_ORDER          // force esp8266 D1 pin mapping
//#define FASTLED_ALLOW_INTERRUPTS 0            // fully disable interrupts while writing out led data
#define FASTLED_INTERRUPT_RETRY_COUNT 4       // re-try attempts
#include <FastLED.h>                          // WS2812B LED strip control and effects
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version
#include "painlessMesh.h"
#include <MT_LightControlDefines.h>

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_Mesh";
const String _progVers = "0.312";             // mqtt modes and flickering (due to wifi interrupts and led bit-banging output style on esp8266)

boolean DEBUG_GEN = false;                     // realtime serial debugging output - general
boolean DEBUG_OVERLAY = false;                // show debug overlay on leds (eg. show segment endpoints, center, etc.)
boolean DEBUG_MESHSYNC = false;               // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
boolean DEBUG_COMMS = false;                   // realtime serial debugging output - comms
boolean DEBUG_USERINPUT = false;              // realtime serial debugging output - user input

boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;              // this should init false, then get activated by input - on/off true/false
bool shouldSaveSettings = false; // flag for saving data
bool runonce = true; // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;                 // just in case  - using FastLED.delay instead..
#define UPDATES_PER_SECOND 120                // main loop FastLED show delay fps  //100

/*----------------------------pins----------------------------*/
const int _ledDOutPin = 14;                   // DOut 0 -> LED strip 0 DIn
//const int _i2cSDApin = 4;                     // SDA (D2)
//const int _i2cSCLpin = 5;                     // SCL (D1)

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 7;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7, 8 }; // test basic, tap bt to cycle around a few mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 4;                    // current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 3;                   // the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
String modeName[_modeNum] = { "Glow", "Sunrise", "Morning", "Day", "Working", "Evening", "Sunset", "Night", "Effect" };
const int _colorTempNum = 3;                  // 3 color temperature sub-modes for now
int _colorTempCur = 1;                        // current colour temperature
String colorTempName[_colorTempNum] = { "Warm", "Standard", "CoolWhite" }; // color temperature sub-mode names for the main "Working" mode.

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
const int _ledNum = 96;                      // 95 + 1 LEDs
const int _segmentTotal = 5;                  // Xm strip with LEDs (4 + 1)
const int _ledGlobalBrightness = 255;         // global brightness
int _ledGlobalBrightnessCur = 255;            // current global brightness - adjust this
int _ledBrightnessIncDecAmount = 10;          // the brightness amount to increase or decrease
LED_SEGMENT ledSegment[_segmentTotal] = { 
  { 0, 0, 1 },
  { 1, 12, 12 }, 
  { 13, 47, 35 }, 
  { 48, 60, 13 },
  { 61, 95, 35 }
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

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;
String _modeString = "Glow";
uint32_t id = DEVICE_ID_BRIDGE1;

//bool calc_delay = false;
//SimpleList<uint32_t> nodes;

//Scheduler userScheduler;
//Task blinkNoNodes;
//bool onFlag = false;                          // task to blink the number of active nodes
//#define   BLINK_PERIOD    3000 // milliseconds until cycle repeat
//#define   BLINK_DURATION  100  // milliseconds LED is on for

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG_GEN) { Serial.printf("deskLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (runonce == true) {
    publishState(false);
    publishBrightness(false);
    //publishRGB(false);
    publishMode(false);
    publishColorTemp(false);
    publishDebugOverlayState(false);
    runonce = false;
  }

  if (DEBUG_MESHSYNC) {
    // Reset blink task
//    onFlag = false;
//    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
//    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
  }
  
  if (DEBUG_COMMS) { Serial.printf("--> deskLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  if (DEBUG_COMMS) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }

  if (DEBUG_MESHSYNC) {
    // Reset blink task
//    onFlag = false;
//    blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
//    blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
//   
//    nodes = mesh.getNodeList();
//  
//    Serial.printf("Num nodes: %d\n", nodes.size());
//    Serial.printf("Connection list:");
//  
//    SimpleList<uint32_t>::iterator node = nodes.begin();
//    while (node != nodes.end()) {
//      Serial.printf(" %u", *node);
//      node++;
//    }
//    Serial.println();
//    calc_delay = true;
  }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG_COMMS) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG_COMMS) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}


/*----------------------------TEMP----------------------------*/
//int _builtInLedState = LOW;
//unsigned long _builtInLedPreviousMillis = 0;
//const long _builtInLedInterval = 500;

/*----------------------------MAIN----------------------------*/
void setup() {
  
  Serial.begin(115200);
  
  Serial.println();
  Serial.print(_progName);
  Serial.print(" v");
  Serial.print(_progVers);
  Serial.println();
  Serial.print("..");
  Serial.println();
  
//  pinMode(LED_BUILTIN, OUTPUT);
//  _builtInLedState = HIGH;  // off
//  digitalWrite(LED_BUILTIN, _builtInLedState);

  delay(3000);                                // give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();
  setupMesh();

  //WIFI.setSleepMode(WIFI_NONE_SLEEP);
  
  //everything done? ok then..
  Serial.print(F("Setup done"));
  Serial.println("-----");
  Serial.print(F("Device Node ID is "));
  String s = String(mesh.getNodeId());
  Serial.println(s);
  Serial.println("-----");
  Serial.println("");

  delay(1500);
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    if (DEBUG_GEN) { }
    _firstTimeSetupDone = true;               // need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  mesh.update();
//  userScheduler.execute();
  
  loopUserInputs();
  loopModes();
  
  if (DEBUG_OVERLAY) {
    //FastLED.clear();
    checkSegmentEndpoints();
    //showColorTempPx();
    leds[0] = CRGB::Red;
  } else {
    leds[0] = CRGB::Black;
  }
  
  if (DEBUG_MESHSYNC) {
//    if (onFlag) { leds[1] = CRGB::Black; } 
//    else { leds[1] = CRGB::Red; }
  }

//  unsigned long builtInLedCurrentMillis = millis();
//  if (mesh.isConnected(mesh.getNodeId())) { //DEBUG_COMMS && 
//    if (builtInLedCurrentMillis - _builtInLedPreviousMillis >= _builtInLedInterval) {
//      _builtInLedPreviousMillis = builtInLedCurrentMillis;
//      if (_builtInLedState == LOW) {
//        _builtInLedState = HIGH;  // Note that this switches the LED *off*
//      } else {
//        _builtInLedState = LOW;  // Note that this switches the LED *on*
//      }
//      digitalWrite(LED_BUILTIN, _builtInLedState);
//    }
//  }

      
  FastLED.show();                             // send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);                      // using FastLED.delay instead..
  
}

