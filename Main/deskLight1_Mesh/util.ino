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
void setGlobalBrightness(uint8_t gb) {
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
  strip.SetBrightness(_ledGlobalBrightnessCur);
}

/*
 * Color temperature utils
 */
void setColorTemp(int i) {
  _colorTempCur = i;
  setColorTempHslCur(i);
  colorTempCatch();
}
void setColorTempHslCur(int i) {
  if (i == 0) {
    _rgbColorTempCur = _rgbWarmFluorescent;
  } else if (i == 1) {
    _rgbColorTempCur = _rgbStandardFluorescent;
  } else if (i == 2) {
    _rgbColorTempCur = _rgbCoolWhiteFluorescent;
  }
}
void cycleColorTemp() {
  _colorTempCur += 1;
  colorTempCatch();
}
void colorTempCatch() {
  if ( (_colorTempCur >= _colorTempNum) || (_colorTempCur < 0) ) { 
    // rollover
    _colorTempCur = 0; 
    setColorTempHslCur(0); 
  }  
}

/*
 * Alternative map function that uses float (instead of int)
 */
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
