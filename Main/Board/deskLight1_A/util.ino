/*----------------------------util----------------------------*/

/*
  DS3231 - kick the interrupt
  TEMP - just to make sure interrupt output pin SQW gets turned on
  enables interrupt pin, alarm 2 and alarm
 */
void DS3231kickInterrupt() {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);               //control location
  Wire.write(B00000111);          //this!
  Wire.endTransmission();
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  fill_solid( leds, LED_NUM, CRGB(0,0,0));
  
  leds[ledSegment[0].first] = CRGB(255, 0, 0);
  leds[ledSegment[0].last] = CRGB(255, 0, 0);
  
  leds[ledSegment[1].first] = CRGB(0, 255, 0);
  leds[ledSegment[1].last] = CRGB(0, 255, 0);
  
  leds[ledSegment[2].first] = CRGB(0, 0, 255);
  leds[ledSegment[2].last] = CRGB(0, 0, 255);
  
  leds[ledSegment[3].first] = CRGB(0, 255, 0);
  leds[ledSegment[3].last] = CRGB(0, 255, 0);
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  leds[0] = _colorTempCur; //show indicator pixel    //colorTempCur from "deskLight1aSettings.h"
  //leds[0] = TEMPERATURE_1; //show indicator pixel
}

/*
 Get Voltage
 usage | Serial.println( getVoltage(), DEC );
 This compares Vcc against internal 1.1v reference and returns the result in mV (eg. 5000mV = 5v)
 ATmega168 & 328 chips can use this trick to read their own voltage rails
 https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
 */
long getVoltage() { 
  long result;                                            // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2);                                               // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC);                                    // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = 1126400L / result;                              // Back-calculate AVcc in mV
  return result;
}

