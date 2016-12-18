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
const String _progVers = "0.21";
/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
#define DEBUG 1                           //
#ifdef DEBUG
String inputString = "";                  // a string to hold incoming data
boolean stringComplete = false;           // whether the string is complete
#endif
const int _mainLoopDelay = 0;             //just in case

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

CRGB leds[LED_NUM];                       //global RGB array
//CRGBArray<LED_NUM> leds;                 //array RGBSet ..neater   ..erm, if it worksS
/*
 * 0-11
 * 12-46
 * 47-59
 * 60-94
 */
const int _segmentTotal = 4;
//#define SEGMENT_TOTAL 4
//struct ???
int _segmentStart[_segmentTotal] = { 0, 12, 47, 60 };
int _segmentLength[_segmentTotal] = { 12, 35, 12, 34 };
int _segmentEnd[_segmentTotal] = { 12, 47, 60, 95 };

int _ledState = LOW;                      //use to toggle LOW/HIGH (ledState = !ledState)
#define LED_GLOBAL_BRIGHTNESS 255                    //global brightness

#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent

CHSV startColor( 144, 70, 64 );
CHSV endColor( 31, 71, 69 );

CRGB startColor_RGB( 3, 144, 232 );
CRGB endColor_RGB( 249, 180, 1 );


/*----------------------------MAIN----------------------------*/
boolean _firstTimeSetupDone = false;      //starts false
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
  setupButtons();
  //setupTouchSensors();


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

  DS3231kickInterrupt();  //TEMP
}

void loop() {
//MOVED - err.. now shouldn't this be used in the main loop section? come on then, move along, move along now.. nothing to see here
  if(_firstTimeSetupDone == false) {
    ifdef DEBUG
      //RTC.promptForTimeAndDate(Serial);
      _firstTimeSetupDone = true;
    endif
  }

//  if (stringComplete) {
//    Serial.println(inputString);
//    // clear the string:
//    inputString = "";
//    stringComplete = false;
//  }

  button0();
  //touchSensors();

  //..this is messy
  if(_mode == 0) {
    mode0();
  } 
  else if(_mode == 1) {
    if(_button0Toggled == true) {
      mode1();
    } 
    else if(_button0Toggled == false) {
      fadeToBlackBy( leds, LED_NUM, 30); //fade everything to black
    }
  } 
  else if(_mode == 2) {
    mode2();
  }

  //clock extras
  showTime();
  sunRiseSetCheck();  //call here so only calling once per frame
  sunRise();
  sunSet();

  FastLED.show();             //send all the data to the strips
  FastLED.delay(8);
  //
  delay(_mainLoopDelay);
}

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

