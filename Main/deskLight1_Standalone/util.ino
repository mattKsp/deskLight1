/*----------------------------util----------------------------*/

/*
 * Mode utils
 */
void incrementPresetSlot() {
  _modePresetSlotCur += 1;
  incDecPresSlot_prtB();
}
void decrementPresetSlot() {
  _modePresetSlotCur -= 1;
  incDecPresSlot_prtB();
}
void incDecPresSlot_prtB() {
  if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
  _modeCur = _modePreset[_modePresetSlotCur];
}

/*
 * Golbal brightness utils
 */
void increaseBrightness() {
  _ledGlobalBrightnessCur += _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void decreaseBrightness() {
  _ledGlobalBrightnessCur -= _ledBrightnessIncDecAmount;
  brightnessRolloverCatch();
}
void brightnessRolloverCatch() {
  if(_ledGlobalBrightnessCur > 255) {
    _ledGlobalBrightnessCur = 255;
  } else if(_ledGlobalBrightnessCur < 0) {
    _ledGlobalBrightnessCur = 0;
  }
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  fill_solid( leds, _ledNum, CRGB(0,0,0));
  
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
  leds[0] = _colorTempCur;                  //show indicator pixel
  //leds[0] = TEMPERATURE_1;                  //show indicator pixel
}

/*
 Get Voltage
 usage | Serial.println( getVoltage(), DEC );
 This compares Vcc against internal 1.1v reference and returns the result in mV (eg. 5000mV = 5v)
 ATmega168 & 328 chips can use this trick to read their own voltage rails
 https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
 */
long getVoltage() { 
  long result;                              // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2);                                 // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC);                      // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = 1126400L / result;               // Back-calculate AVcc in mV
  return result;
}

