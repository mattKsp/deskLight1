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
/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
#define DEBUG 1                             //comment/un-comment
//#define SET_TIME_BY_SERIAL 1              //(needs debug for serial) 'un-comment' to get a serial prompt at startup to set the time. set time. then 'comment' and upload again
#define SET_ALARM1_BY_SERIAL 1            //(needs debug for serial) 'un-comment' to get a serial prompt at startup to set Alarm 1
//#define SET_ALARM2_BY_SERIAL 1            //(needs debug for serial) 'un-comment' to get a serial prompt at startup to set Alarm 2

const String _progName = "deskLight1_A";
const String _progVers = "0.23";
const int _mainLoopDelay = 0;               //just in case
boolean _firstTimeSetupDone = false;        //starts false
boolean _onOff = false;                     //this should init false, then get activated by input
#ifdef DEBUG
String _inputString = "";                   // a string to hold incoming data
boolean stringComplete = false;             // whether the string is complete
#endif

/*----------------------------modes----------------------------*/
typedef struct {
  String mName;                             //added 'm' prefix to orig as 'name' has a.. wait for it.. a 'name' clash with.. 'name' ..bang, crash!
  boolean isStatic;                         //moving(false) or static(true) //everything is vibration
} MODE_INFO;
const int _modeNum = 9;
const int _modePresetSlotNum = 3;
int _modePreset[_modePresetSlotNum] = { 0, 4, 6 }; //test basic, tap bt to cycle around 3 mode slots   //expand to array or struct later for more presets
MODE_INFO modeInfo[_modeNum] = {
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
int _modeCur = 0;                           //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                 //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
const int _DS3231interruptPin = 3;          //the alarm return from the DS3231 (INT/SQW pin on chip)
DS3231_Simple RTC;                          //init realtime clock
#define DS3231_I2C_ADDRESS 0x68             //default
boolean _sunRiseEnabled = false;
volatile boolean _sunRiseTriggered = false;
boolean _sunSetEnabled = false;
volatile boolean _sunSetTriggered = false;
volatile boolean _sunRiseSetTriggered = false;
int _sunRiseStateCur = 0;                   //current sunrise internal state (beginning, rise, end)
int _sunSetStateCur = 0;                   //current sunset internal state (beginning, fall, end)

/*----------------------------buttons----------------------------*/
const int _button0Pin = 2;                  //#define BUTTON_0_PIN 2 _button0Pin
const unsigned long _buttonDebounceTime = 5; //unsigned long (5ms)
Bounce _button0 = Bounce();                 //Instantiate a Bounce object
boolean _button0Toggled = false;

/*----------------------------touch sensors----------------------------*/
//int _touchSensorsTotal = 3;
boolean _touchSensorToggled[3];

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledPin = 13;                     //built-in LED
const int _ledDOutPin = 6;                  //DOut -> LED strip DIn
const int _ledNum = 150;                    //5m strip with 150 LEDs
const int _segmentTotal = 4;                //more later..
const int _ledGlobalBrightness = 255;          //global brightness
#define UPDATES_PER_SECOND 100              //main loop FastLED show delay
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

CRGB leds[_ledNum];                         //global RGB array
int _ledState = LOW;                        //use to toggle LOW/HIGH (ledState = !ledState)
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
int _colorTempCur = 5;                      //current colour temperature

/*----------------------------MAIN----------------------------*/
void setup() {
  
  #ifdef DEBUG
    Serial.begin(9600);
    _inputString.reserve(200);              // reserve 200 bytes for the inputString:
    Serial.println();
    Serial.print(_progName);
    Serial.print(" ");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  #endif
  
  RTC.begin();                              //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  //RTC.disableAlarms();
  
  setupInterrupts();
  delay(3000);                              //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();

  #ifdef DEBUG
    #ifdef SET_TIME_BY_SERIAL
      RTC.promptForTimeAndDate(Serial);
    #endif
    #ifdef SET_ALARM1_BY_SERIAL
      RTC.disableAlarms();
      promptForAlarm1(Serial);
    #endif
    #ifdef SET_ALARM2_BY_SERIAL
      RTC.disableAlarms();
      promptForAlarm1(Serial);
    #endif
    //RTC.disableAlarms();
    //setSunRise(1, 7, 30); //@07:30
    //setSunSet(1, 13, 16); //@18:45
  #endif

  DS3231kickInterrupt();  //TEMP util
}

void loop() {
  
  if(_firstTimeSetupDone == false) {
    #ifdef DEBUG
    //
    #endif
    _firstTimeSetupDone = true;           //need this for stuff like setting sunrise, cos it needs the time to have been set
  }

  #ifdef DEBUG
    if (stringComplete) {
      Serial.println(_inputString);
      _inputString = "";                    //clear the string:
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

  FastLED.show();                           //send all the data to the strips
  //FastLED.delay(8);
  FastLED.delay(1000 / UPDATES_PER_SECOND);
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
    char inChar = (char)Serial.read();      // get the new byte:
    _inputString += inChar;                 // add it to the inputString:
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
#endif

