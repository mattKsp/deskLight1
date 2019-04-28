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
// board running at [ this-> 80mhz || 1600mhz ]
#include <EEPROM.h>                           // a few saved settings
#include <FastLED.h>                          // still using 'beatsin8' for breathing
#include <NeoPixelBus.h>
#include "Seeed_MPR121_driver.h"              // Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121) - using edited version
#include "painlessMesh.h"
#include <MT_LightControlDefines.h>

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_Mesh";
const String _progVers = "0.4";               // replacing FastLED with NeoPixelBus (just for ESP8266)

boolean DEBUG_GEN = true;                     // realtime serial debugging output - general
boolean DEBUG_OVERLAY = false;                // show debug overlay on leds (eg. show segment endpoints, center, etc.)
boolean DEBUG_MESHSYNC = false;               // show painless mesh sync by flashing some leds (no = count of active mesh nodes) 
boolean DEBUG_COMMS = true;                   // realtime serial debugging output - comms
boolean DEBUG_USERINPUT = true;               // realtime serial debugging output - user input

boolean _firstTimeSetupDone = false;          // starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = true;              // this should init false, then get activated by input - on/off true/false
bool shouldSaveSettings = false; // flag for saving data
bool runonce = true; // flag for sending states when first mesh conection
//const int _mainLoopDelay = 0;                 // just in case  - using FastLED.delay instead..

/*----------------------------pins----------------------------*/
// NeoPixelBus - For Esp8266, the Pin is omitted and it uses GPIO3(RX) due to DMA hardware use.

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

/*----------------------------buttons----------------------------*/

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNum = 96;                       // 95 + 1 LEDs
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
int _ledState = LOW;                          // use to toggle LOW/HIGH (ledState = !ledState)

#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent

const uint16_t PixelCount = 96;               // NeoPixelBus - 95 + 1 LEDs
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

uint8_t _gHue2 = 0;

RgbColor _rgbStartColor( 3, 144, 232 );
RgbColor _rgbEndColor( 249, 180, 1 );

RgbColor _rgbRed(255, 0, 0);
RgbColor _rgbGreen(0, 255, 0);
RgbColor _rgbBlue(0, 0, 255);
RgbColor _rgbYellow(255, 255, 0);
RgbColor _rgbFuchsia(255, 0, 128);
RgbColor _rgbOrange(255, 165, 0);
RgbColor _rgbViolet(148, 0, 211);
RgbColor _rgbTeal(0, 128, 128);
RgbColor _rgbPink(255, 105, 180);
RgbColor _rgbWhite(255, 250, 255);
RgbColor _rgbGlow(32, 32, 32);
RgbColor _rgbBlack(0, 0, 0);

//HslColor color(_gHue / 255.0f, saturationValue, lightnessValue); // 0.0 to 1.0
HslColor _colorHSL(0.25f, 0.5f, 0.5f);
HslColor _hslStartColor( 144, 70, 64 );
HslColor _hslEndColor( 31, 71, 69 );

/*----------------------------Mesh----------------------------*/
painlessMesh  mesh;
String _modeString = "Day";
uint32_t id = DEVICE_ID_BRIDGE1;

void receivedCallback(uint32_t from, String &msg ) {
  if (DEBUG_GEN) { Serial.printf("deskLight1_Mesh: Received from %u msg=%s\n", from, msg.c_str()); }
  receiveMessage(from, msg);
}

void newConnectionCallback(uint32_t nodeId) {
  if (runonce == true) {
    publishState(false);
    publishBrightness(false);
    publishMode(false);
    publishColorTemp(false);
    publishDebugGeneralState(false);
    publishDebugOverlayState(false);
    publishDebugMeshsyncState(false);
    publishDebugCommsState(false);
    runonce = false;
  }

  if (DEBUG_MESHSYNC) { }
  
  if (DEBUG_COMMS) { Serial.printf("--> deskLight1_Mesh: New Connection, nodeId = %u\n", nodeId); }
}

void changedConnectionCallback() {
  if (DEBUG_COMMS) { Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str()); }

  if (DEBUG_MESHSYNC) { }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  if (DEBUG_COMMS) { Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset); }
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  if (DEBUG_COMMS) { Serial.printf("Delay to node %u is %d us\n", from, delay); }
}


/*----------------------------MAIN----------------------------*/
void setup() 
{
  // Wemos D1 - GPIO 3 (RX) - swap the pin from serial to a GPIO.
  pinMode(3, FUNCTION_3); // FUNCTION_0 = swap back
  
  Serial.begin(115200);
  
  Serial.println();
  Serial.print(_progName);
  Serial.print(" v");
  Serial.print(_progVers);
  Serial.println();
  Serial.print("..");
  Serial.println();
  
  delay(3000);                                // give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();
  setupMesh();

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
  
  loopUserInputs();
  loopModes();
  
  if (DEBUG_OVERLAY) {
    checkSegmentEndpoints();
    strip.SetPixelColor(0, _rgbRed);
  } else {
    strip.SetPixelColor(0, _rgbBlack);
  }
  
  if (DEBUG_MESHSYNC) { }

  strip.Show();
  //delay(_mainLoopDelay);
  
}
