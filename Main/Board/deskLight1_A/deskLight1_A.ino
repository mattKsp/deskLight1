/*
    'deskLight1 A' by Thurstan. LED strip desk light.
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

/*
 * Arduino Pro Mini
 * - 5v
 * - ATmega328
 * - 16mhz clock speed
 * - 32KB Flash
 * - 2KB SRAM
 * - 1KB EEPROM
 * 1x LED strip, WS2812B (GRB)
 * 1x touch strip (2x copper triangular strips combined to make one touch sensor)
 * 1x button ???
 * 
 * https://github.com/FastLED/FastLED/wiki/Pixel-reference
 */

/*
 * on the subject of including libraries..
 * 
 * "Note the use of quotes (") rather than angle brackets (<). 
 * This specifies that the file to be included is in the same folder as the source file. 
 * When you include a library you should use angle brackets. 
 * What makes this more fun is that if you do use quotes to include a library it will 
 * still work due to Arduino's compilation process, although its bad practice."
 * 
 * http://allgood38.io/understand-arduino-development.html
 */

/*----------------------------|----------------------------*/
#include <EEPROM.h>           //a few saved settings
//#include "Wire.h"
#include <DS3231_Simple.h>    //DS3231 realtime clock (with AT24C32 memory backback)
#include <Bounce2.h>          //buttons with de-bounce
#include <CapacitiveSensor.h>
#include "FastLED.h"          //should this use <> ???

/*----------------------------system----------------------------*/
const String _progName = "deskLight1_A";
const String _progVers = "0.1";
boolean _debug = true;
boolean _serialEnabled = true;   //these Enableds are only used during runtime 'cept this one!!! obviously
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int _mainLoopDelay = 0;   //just in case
int _mode = 2;
/*
 * 0 = temperature test
 * 1 = gradient test
 * 2 = preset test0
 * 
 */
int _mode0_sub = 0;
//int _mode1_sub = 0; //uses _button0Toggled instead
int _mode2_sub = 0;

/*-----------------RTC (DS3231 and AT24C32) on I2C------------------*/
//DS3231 & AT24C32 I2C max transmission speed 400KHz
DS3231_Simple RTC;  //init realtime clock
#define DS3231_I2C_ADDRESS 0x68 // default, apparently cannot be changed ???
//#define AT24C32_I2C_ADDRESS 0x57  //memory module 32K //address can be changed but means shorting stuff..
//#define DS3231_I2C_SDA_PIN A4
//#define DS3231_I2C_SCL_PIN A5
#define DS3231_INTERRUPT_PIN 3   //the alarm return from the DS3231 (INT/SQW pin on chip)

boolean _sunRiseEnabled = false;
volatile boolean _sunRiseTriggered = false;
boolean _sunSetEnabled = false;
volatile boolean _sunSetTriggered = false;
volatile boolean _sunRiseSetTriggered = false;

/*----------------------------buttons----------------------------*/
boolean _buttonsEnabled = true;
const unsigned long _buttonDebounceTime = 5;  //unsigned long (5ms)
boolean _button0Toggled = false;
const int _button0Pin = 2;  //#define BUTTON_PIN 3
Bounce _button0 = Bounce(); //Instantiate a Bounce object

/*----------------------------touch sensors----------------------------*/
boolean _touchSensorsEnabled = true;

/*----------------------------LED----------------------------*/
boolean _ledStripsEnabled = true;
//boolean _ledSystemEnabled = true; //probably won't use this
#define LED_PIN 13            //built-in LED
#define DATA_PIN 6            //DOut -> LED strip DIn
#define NUM_LEDS 150          //5m strip with 150 LEDs

CRGB leds[NUM_LEDS];          //global RGB array
//CRGBArray<NUM_LEDS> leds;       //array RGBSet ..neater   ..erm, if it worksS
/*
 * 0-11
 * 12-46
 * 47-59
 * 60-94
 */
const int _segmentTotal = 4;
int _segmentStart[_segmentTotal] = { 0, 12, 47, 60 };
int _segmentLength[_segmentTotal] = { 12, 35, 12, 34 };
int _segmentEnd[_segmentTotal] = { 12, 47, 60, 95 };

#define BRIGHTNESS 255         //global brightness
int _ledState = LOW;             //use to toggle LOW/HIGH (ledState = !ledState)

// FastLED provides these pre-conigured incandescent color profiles:
//     Candle, Tungsten40W, Tungsten100W, Halogen, CarbonArc,
//     HighNoonSun, DirectSunlight, OvercastSky, ClearBlueSky,
// FastLED provides these pre-configured gaseous-light color profiles:
//     WarmFluorescent, StandardFluorescent, CoolWhiteFluorescent,
//     FullSpectrumFluorescent, GrowLightFluorescent, BlackLightFluorescent,
//     MercuryVapor, SodiumVapor, MetalHalide, HighPressureSodium,
// FastLED also provides an "Uncorrected temperature" profile
//    UncorrectedTemperature;
#define TEMPERATURE_0 WarmFluorescent
#define TEMPERATURE_1 StandardFluorescent
#define TEMPERATURE_2 CoolWhiteFluorescent
#define TEMPERATURE_3 FullSpectrumFluorescent
#define TEMPERATURE_4 GrowLightFluorescent
#define TEMPERATURE_5 BlackLightFluorescent
#define TEMPERATURE_6 MercuryVapor
#define TEMPERATURE_7 SodiumVapor
#define TEMPERATURE_8 MetalHalide

//CHSV paleBlue( 160, 128, 255);
  // Now...
  //   paleBlue.hue == 160
  //   paleBlue.sat == 128
  //   paleBlue.val == 255

/*
 * color description : Vivid blue
 * HEX triplet #0390e8
 * RGB 3, 144, 232
 * CMYK  99, 38, 0, 9
 * HSV (0-360) 203.1°, 98.7, 91
 * HSV (0-255) 144, 70, 64 (143.9, 69.9125, 64.4583)
 * 
 * color description : Vivid orange
 * HEX triplet #f9b401
 * RGB  249, 180, 1
 * CMYK  0, 28, 100, 2
 * HSV (0-360) 43.3°, 99.6, 97.6
 * HSV (0-255) 31, 71, 69 (30.6708, 70.55, 69.1333)
 * 
 * color description : Light grayish red - might get near white ???
 * HEX triplet f2eeed
 * RGB  242, 238, 237
 * CMYK  0, 2, 2, 5
 * HSV (0-360)  12°, 2.1, 94.9
 * HSV (0-255) 9, 2, 67 (8.5, 1.4875, 67.2208)
 */
  
  CHSV startColor( 144, 70, 64 );
  CHSV endColor( 31, 71, 69 );

  CRGB startColor_RGB( 3, 144, 232 );
  CRGB endColor_RGB( 249, 180, 1 );


/*----------------------------MAIN----------------------------*/
boolean _firstTimeSetupDone = false;  //starts false
void setup() {
  //pinMode(DS3231_INTERRUPT_PIN, INPUT_PULLUP);  //try putting this here, before 3 sec delay, to see if stops interrupt trigger
  if(_serialEnabled) {
    Serial.begin(9600);
    //inputString.reserve(200); //// reserve 200 bytes for the inputString:
    Serial.println();
    Serial.print(_progName);
    Serial.print(" ");
    Serial.print(_progVers);
    Serial.println();
    Serial.print("..");
    Serial.println();
  }
  RTC.begin();  //DS3231 RealTimeClock begin..
  //RTC.formatEEPROM();
  //RTC.disableAlarms();
  
  setupInterrupts();
  
  delay(3000);                //give the power, LED strip, etc. a couple of secs to stabilise
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

  if(_firstTimeSetupDone == false) {
    if(_serialEnabled) {
      //RTC.promptForTimeAndDate(Serial);
      _firstTimeSetupDone = true;
    }
  }

  if(_debug && _serialEnabled) {
//    Serial.print("The time has been set to: ");
//    RTC.printTo(Serial);
//    Serial.println();
    RTC.disableAlarms();
    setSunRise(1, 7, 30); //@07:30
    setSunSet(1, 13, 16); //@18:45
  }

  //just to make sure interrupt output pin SQW gets turned on
  //enable interrupt pin, alarm 2 and alarm 1
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  //#define DS3231_CONTROL 0x0E //control
  Wire.write(0x0E);  //location
  Wire.write(B00000111);
  Wire.endTransmission();
}

void loop() {
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
      fadeToBlackBy( leds, NUM_LEDS, 30); //fade everything to black
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
////
//  // To check the alarms we just ask the clock
//  uint8_t AlarmsFired = RTC.checkAlarms();
//  
//  // Then can check if either alarm is fired (there are 2 alarms possible in the chip)
//  // by using a "bitwise and"
//  if(AlarmsFired & 1)
//  {
//    RTC.printTo(Serial); Serial.println(": First alarm has fired!");
//  }
//  
//  if(AlarmsFired & 2)
//  {
//    RTC.printTo(Serial); Serial.println(": Second alarm has fired!");
//  }
////
  if(_ledStripsEnabled) {
    FastLED.show(); //send all the data to the strips
    FastLED.delay(8);
  }
  //
  delay(_mainLoopDelay);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag
//    // so the main loop can do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}

