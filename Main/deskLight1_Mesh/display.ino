/*----------------------------display----------------------------*/

void setupLEDs() {
  
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_DRAW);  // limit power draw to 2.85A at 5v (with 3A power supply this gives us a bit of head room for board, lights etc.)
  
  FastLED.addLeds<WS2812B, _ledDOutPin, GRB>(leds, _ledNum).setCorrection( TypicalSMD5050 );  // TypicalLEDStrip
  
  _ledGlobalBrightnessCur = _ledGlobalBrightness;
  FastLED.setBrightness(_ledGlobalBrightness);      // set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   // set first temperature

  FastLED.setMaxRefreshRate(12); // WS2812B default is 400
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(_ledNum) ] += CRGB::White;
  }
}

