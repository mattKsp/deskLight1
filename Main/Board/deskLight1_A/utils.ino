/*----------------------------utils----------------------------*/

void DS3231kickInterrupt() {
  //TEMP - just to make sure interrupt output pin SQW gets turned on
  //enable interrupt pin, alarm 2 and alarm 1
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  //#define DS3231_CONTROL 0x0E //control
  Wire.write(0x0E);  //location
  Wire.write(B00000111);         //this!
  Wire.endTransmission();
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints(); 
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
 fill_solid( leds, LED_NUM, CRGB(0,0,0));
 
 leds[_segmentStart[0]] = CRGB(255, 0, 0);
 leds[_segmentEnd[0]] = CRGB(255, 0, 0);

 leds[_segmentStart[1]] = CRGB(0, 255, 0);
 leds[_segmentEnd[1]] = CRGB(0, 255, 0);
 
 leds[_segmentStart[2]] = CRGB(0, 0, 255);
 leds[_segmentEnd[2]] = CRGB(0, 0, 255);
 
 leds[_segmentStart[3]] = CRGB(0, 255, 0);
 leds[_segmentEnd[3]] = CRGB(0, 255, 0);
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

