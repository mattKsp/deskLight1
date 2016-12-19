/*
    'deskLight1_A' by Thurstan. LED strip desk light.
    Copyright (C) 2016  MTS Standish (Thurstan|mattKsp)

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

    https://github.com/mattKsp/
*/

// Tested on Arduino Pro Mini (5v, ATmega328, 16mhz clock speed, 32KB Flash, 2KB SRAM, 1KB EEPROM)


/*----------------------------libraries----------------------------*/
#include <DebugUtils.h>                   //DEBUG_PRINT(str) - tool that spits out message and appropriate stamps for where in the program, what time, etc.
#include <EEPROM.h>                       //a few saved settings
#include <DS3231_Simple.h>                //DS3231 realtime clock (with AT24C32 memory backback)
#include <Bounce2.h>                      //buttons with de-bounce
#include <CapacitiveSensor.h>             //capacitive touch sensors
#include <FastLED.h>                      //WS2812B LED strip control and effects

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_A";
const String _progVers = "0.22";
/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
#define DEBUG 1                           //comment/un-comment
#ifdef DEBUG
String inputString = "";                  // a string to hold incoming data
boolean stringComplete = false;           // whether the string is complete
#endif
const int _mainLoopDelay = 0;             //just in case
boolean _firstTimeSetupDone = false;      //starts false
boolean _onOff = true;                    //TEMP true - this should init false, then get activated by touching something

/*----------------------------modes----------------------------*/
//#define MODE_NUM 9                        //total modes available
const int MODE_NUM = 9;
int _modeCur = 0;                         //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
typedef struct {
  String mName;     //added 'm' prefix to orig as 'name' has a.. wait for it.. a 'name' clash with.. 'name' ..bang, crash!
  boolean isStatic; //moving(false) or static(true) //everything is vibration
} MODE_INFO;
MODE_INFO modeInfo[MODE_NUM] = {
  { "glow", true },
  { "sunrise", false},
  { "morning", false},
  { "day", false},
  { "working", true},
  { "evening", false},
  { "sunset", false},
  { "night", false},
  { "changing", false}
};
//modeInfo[0] = { "glow", true }; //alt
//#define MODE_PRESET_SLOT_NUM 3                        //total mode preset selection slots available
const int MODE_PRESET_SLOT_NUM = 3;
int _modePreset[MODE_PRESET_SLOT_NUM] = { 0, 4, 6 };  //test basic, tap bt to cycle around 3 mode slots   //expand to array or struct later for more presets
int _modePresetSlotCur = 0;                           //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput
//#define MODE_PRESET_NUM 3                           //total mode preset selections available - not in use yet
//int _modePresetCur = 0;                             //..the current preset selection - not in use yet
//eg. _modeCur = _modePreset[_modePresetSlotCur];

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
DS3231_Simple RTC;                        //init realtime clock
#define DS3231_I2C_ADDRESS 0x68           // default, apparently cannot be changed ???
//#define AT24C32_I2C_ADDRESS 0x57          //memory module 32K //address can be changed but means shorting stuff..
//#define DS3231_I2C_SDA_PIN A4
//#define DS3231_I2C_SCL_PIN A5
#define DS3231_INTERRUPT_PIN 3            //the alarm return from the DS3231 (INT/SQW pin on chip)

boolean _sunRiseEnabled = false;
volatile boolean _sunRiseTriggered = false;
boolean _sunSetEnabled = false;
volatile boolean _sunSetTriggered = false;
volatile boolean _sunRiseSetTriggered = false;

/*----------------------------buttons----------------------------*/
const unsigned long _buttonDebounceTime = 5;  //unsigned long (5ms)
boolean _button0Toggled = false;
#define BUTTON_0_PIN 2                    //const int _button0Pin = 2;
Bounce _button0 = Bounce();               //Instantiate a Bounce object

/*----------------------------touch sensors----------------------------*/


/*----------------------------LED----------------------------*/
#define LED_PIN 13                        //built-in LED
#define LED_DOUT_PIN 6                    //DOut -> LED strip DIn
#define LED_NUM 150                       //5m strip with 150 LEDs
//const int LED_NUM = 150;
//ext .h file.. erm.. array bound is not an integer constant before ']' token, so..
//const int LED_NUM = 150;

CRGB leds[LED_NUM];                       //global RGB array
//CRGBArray<LED_NUM> leds;                 //array RGBSet ..neater   ..erm, if it worksS
/*
 * 0-11
 * 12-46
 * 47-59
 * 60-94
 */
//const int _segmentTotal = 4;
//#define SEGMENT_TOTAL 4                   //more later..
const int SEGMENT_TOTAL = 4;
//struct ???
//int _segmentStart[SEGMENT_TOTAL] = { 0, 12, 47, 60 };
//int _segmentLength[SEGMENT_TOTAL] = { 12, 35, 12, 34 };
//int _segmentEnd[SEGMENT_TOTAL] = { 12, 47, 60, 95 };

typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;

LED_SEGMENT ledSegment[SEGMENT_TOTAL] = { 
  { 0, 11, 12 }, 
  { 12, 46, 35 }, 
  { 47, 59, 12 },
  { 60, 94, 34 }
};
//ledSegment[0] = { 0, 0, 0 };
//ledSegment[0].first

int _ledState = LOW;                                //use to toggle LOW/HIGH (ledState = !ledState)
#define LED_GLOBAL_BRIGHTNESS 255                   //global brightness

#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                               //current colour temperature

CHSV startColor( 144, 70, 64 );       //these stay here cos using CHSV from FastLED library
CHSV endColor( 31, 71, 69 );

CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );


/*----------------------------MAIN----------------------------*/
void setup() {
  
  #ifdef DEBUG
    Serial.begin(9600);
    //inputString.reserve(200);             // reserve 200 bytes for the inputString:
    Serial.println();
    Serial.print(_progName);
    Serial.print(" ");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  #endif
  
  RTC.begin();                            //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  //RTC.disableAlarms();
  
  setupInterrupts();
  delay(3000);                            //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();


//  DateTime MyTimestamp;
//  //   Saturday 17 Dec 2016 at 10:42 and 0 Secs
//  MyTimestamp.Day    = 17;
//  MyTimestamp.Month  = 12;
//  MyTimestamp.Year   = 16; 
//  MyTimestamp.Hour   = 10;
//  MyTimestamp.Minute = 42;
//  MyTimestamp.Second = 0;
//  
//  RTC.write(MyTimestamp);

  #ifdef DEBUG
//    Serial.print("The time has been set to: ");
//    RTC.printTo(Serial);
//    Serial.println();
    RTC.disableAlarms();
    setSunRise(1, 7, 30); //@07:30
    setSunSet(1, 13, 16); //@18:45
  #endif

  DS3231kickInterrupt();  //TEMP util
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    #ifdef DEBUG
      //RTC.promptForTimeAndDate(Serial);
      _firstTimeSetupDone = true;         //need this for stuff like setting sunrise, cos it needs the time to have been set
    #endif
  }

  #ifdef DEBUG
    if (stringComplete) {
      Serial.println(inputString);
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
  #endif

  loopUserInputs();
  loopModes();
  //clock extras
  //showTime();
  //sunRiseSetCheck();  //call here so only calling once per frame
  //sunRise();
  //sunSet();

  FastLED.show();             //send all the data to the strips
  FastLED.delay(8);
  //
  delay(_mainLoopDelay);
}


/*----------------------------Serial----------------------------*/
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
#ifdef DEBUG
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
#endif

