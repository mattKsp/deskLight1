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

/* 
 use if defined statements and comment out to disable debug mode 
 this will remove all debug code when compiling rather than just switching off
 for now only use serial when in debug 
*/
#define DEBUG 1                           //comment/un-comment
#ifdef DEBUG     
//#define SET_TIME_BY_SERIAL 1              //(needs debug for serial) 'un-comment' to get a serial prompt at startup to set the time. set time. then 'comment' and upload again
#endif
const int _button0Pin = 2;                //#define BUTTON_0_PIN 2 _button0Pin
const int _button1Pin = 3;                //#define BUTTON_1_PIN 2 _button1Pin
//const int _DS3231interruptPin = 3;        //the alarm return from the DS3231 (INT/SQW pin on chip)
const int _ledPin = 13;                   //built-in LED
const int _ledDOutPin = 6;                //DOut -> LED strip DIn
//const int _capSenseSendPin = 7;           //capacitive touch sensor (send)
//const int _capSense0Pin = 8;              //on/off (receive) - note: all other touch sensors will trigger 'on' if 'off', aswell as this pin..
//const int _capSense1Pin = 9;              //mode - capacitive touch sensor (receive)
//const int _capSense2Pin = 10;              //sub-mode - capacitive touch sensor (receive) - not in use yet
//const int _capSense3Pin = 11;             //brightness up - capacitive touch sensor (receive)
//const int _capSense4Pin = 12;             //brightness down - capacitive touch sensor (receive)

/*----------------------------libraries----------------------------*/
//#include <EEPROM.h>                       //a few saved settings
//#include <DS3231_Simple.h>                //DS3231 realtime clock (with AT24C32 memory backback)
#include <Bounce2.h>                      //buttons with de-bounce
//#include <CapacitiveSensor.h>             //capacitive touch sensors
#include <FastLED.h>                      //WS2812B LED strip control and effects

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_A";
const String _progVers = "0.26";            //moved DS3231 to leaningBookShelvesLight1_A and swapped touch for buttons (until can get shielded wire etc.)
//const int _mainLoopDelay = 0;               //just in case  - using FastLED.delay instead..
boolean _firstTimeSetupDone = false;        //starts false //this is mainly to catch an interrupt trigger that happens during setup, but is usefull for other things
volatile boolean _onOff = false;            //this should init false, then get activated by input - on/off true/false
#ifdef DEBUG
String _inputString = "";                   // a string to hold incoming data
boolean stringComplete = false;             // whether the string is complete
#endif

/*----------------------------modes----------------------------*/
const int _modeNum = 9;
const int _modePresetSlotNum = 7;
int _modePreset[_modePresetSlotNum] = { 0, 2, 3, 4, 5, 7, 8 }; //test basic, tap bt to cycle around a few mode slots   //expand to array or struct later for more presets
volatile int _modeCur = 0;                  //current mode in use - this is not the var you are looking for.. try _modePresetSlotCur
int _modePresetSlotCur = 0;                 //the current array pos (slot) in the current preset, as opposed to..      //+/- by userInput

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
//DS3231_Simple RTC;                          //init realtime clock
//#define DS3231_I2C_ADDRESS 0x68             //default - only used for interrupt kick
////boolean _sunRiseEnabled = false;
////volatile boolean _sunRiseTriggered = false;
////boolean _sunSetEnabled = false;
////volatile boolean _sunSetTriggered = false;
////volatile boolean _sunRiseSetTriggered = false;
//int _sunRiseStateCur = 0;                   //current sunrise internal state (beginning, rise, end)
//int _sunSetStateCur = 0;                    //current sunset internal state (beginning, fall, end)

///*----------------------------buttons----------------------------*/
const unsigned long _buttonDebounceTime = 5; //unsigned long (5ms)
Bounce _button0 = Bounce();                 //Instantiate a Bounce object
Bounce _button1 = Bounce();                 //Instantiate a Bounce object
boolean _button0Toggled = false;
boolean _button1Toggled = false;

/*----------------------------touch sensors----------------------------*/
//CapacitiveSensor _touch0 = CapacitiveSensor(_capSenseSendPin,_capSense0Pin);  //on/off
//CapacitiveSensor _touch1 = CapacitiveSensor(_capSenseSendPin,_capSense1Pin);  //mode
//CapacitiveSensor _touch2 = CapacitiveSensor(_capSenseSendPin,_capSense2Pin);  //sub-mode
//CapacitiveSensor _touch3 = CapacitiveSensor(_capSenseSendPin,_capSense3Pin);  //brightness up
//CapacitiveSensor _touch4 = CapacitiveSensor(_capSenseSendPin,_capSense4Pin);  //brightness down

//byte _touchSensorRes = 50;     //50          //sample/sensor resolution - higher is better but slower to read
//long _touchSensorThreshold = 2000;           //unsigned long   //1 for all at the moment
//const long _touchDeBounceInterval = 1000;   //500                    //interval to de-bounce in milliseconds    //const int 
//long _touchPrevMillis[5] = { 0, 0, 0, 0, 0 };                       //how long between 'bounces' //unsigned long
//boolean _touchToggled[5] = { false, false, false, false, false };

/*----------------------------LED----------------------------*/
typedef struct {
  byte first;
  byte last;
  byte total;
} LED_SEGMENT;
const int _ledNum = 150;                    //5m strip with 150 LEDs
//const int _ledNum = 40;     //TEMP testing - 55 on roll, using 40
const int _segmentTotal = 4;                //more later..
const int _ledGlobalBrightness = 255;          //global brightness
int _ledGlobalBrightnessCur = 255;              //current global brightness - adjust this
int _ledBrightnessIncDecAmount = 10;            //the brightness amount to increase or decrease
#define UPDATES_PER_SECOND 120              //main loop FastLED show delay  //100
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
  
  //RTC.begin();                              //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  //setupInterrupts();
  delay(3000);                              //give the power, LED strip, etc. a couple of secs to stabilise
  setupLEDs();
  setupUserInputs();
//
//  #ifdef DEBUG
//    #ifdef SET_TIME_BY_SERIAL
//      RTC.promptForTimeAndDate(Serial);
//    #endif
//    Serial.println(F("Current date/time = "));
//    RTC.printTo(Serial);
//  #endif
//  
  //setSunRise(9, 30);      //TEMP
  //DS3231kickInterrupt();  //TEMP util
  
  #ifdef DEBUG
  //everything done? ok then..
    Serial.print(F("Setup done"));
    Serial.println();
  #endif
  blinkStatusLED();
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
//    // To check the alarms we just ask the clock
//    uint8_t AlarmsFired = RTC.checkAlarms();
//    // Then can check if either alarm is fired (there are 2 alarms possible in the chip)
//    // by using a "bitwise and"
//    if(AlarmsFired & 1) {
//      RTC.printTo(Serial); Serial.println(": First alarm has fired!");
//    }
//    if(AlarmsFired & 2) {
//      RTC.printTo(Serial); Serial.println(": Second alarm has fired!");
//    }
  #endif

  loopUserInputs();
  loopModes();
  
  //clock extras
  //showTime();

  FastLED.show();                           //send all the data to the strips
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //
  //delay(_mainLoopDelay);                  //using FastLED.delay instead..
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

