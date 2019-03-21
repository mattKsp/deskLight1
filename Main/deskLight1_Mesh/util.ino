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
  if(_modePresetSlotCur >= _modePresetSlotNum || _modePresetSlotCur < 0) { 
    _modePresetSlotCur = 0;                     // TEMP rollover catch
    }  
  _modeCur = _modePreset[_modePresetSlotCur];
}

/*
 * Golbal brightness utils
 */
void setGlobalBrightness(int gb) {
  //use this to achieve an override from the mesh, eg. to match levels
  _ledGlobalBrightnessCur = gb;
  brightnessRolloverCatch();
}
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
  leds[1] = _colorTempCur;                    // show indicator pixel
  //leds[0] = TEMPERATURE_1;                    // show indicator pixel
}


