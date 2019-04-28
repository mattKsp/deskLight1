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
 * Global brightness utils
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
 * Color temperature utils
 */
void setColorTemp(int i) {
  _colorTempCur = i;
  colorTempCatch();
}
void cycleColorTemp() {
  _colorTempCur += 1;
  colorTempCatch();
}
void colorTempCatch() {
  if ( (_colorTempCur >= _colorTempNum) || (_colorTempCur < 0) ) 
  { _colorTempCur = 0; }  // rollover
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  strip.ClearTo(_rgbBlack);
  
  strip.SetPixelColor(ledSegment[1].first, _rgbRed);
  strip.SetPixelColor(ledSegment[1].last, _rgbRed);
  
  strip.SetPixelColor(ledSegment[2].first, _rgbGreen);
  strip.SetPixelColor(ledSegment[2].last, _rgbGreen);
  
  strip.SetPixelColor(ledSegment[3].first, _rgbBlue);
  strip.SetPixelColor(ledSegment[3].last, _rgbBlue);
  
  strip.SetPixelColor(ledSegment[4].first, _rgbRed);
  strip.SetPixelColor(ledSegment[4].last, _rgbRed);
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  //leds[1] = _colorTempCur;                    // show indicator pixel
  //leds[0] = TEMPERATURE_1;                    // show indicator pixel
}

/*
 * Alternative map function that uses float (instead of int)
 */
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
