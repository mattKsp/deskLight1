/*----------------------------display----------------------------*/

void setupLEDs() {
  
  //system LEDs
  pinMode(_ledPin, OUTPUT);                 //Setup the internal LED on pin 13
  digitalWrite(_ledPin, _ledState);

  //FastLED
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2850);  //limit power draw to 2.85A at 5v (with 3A power supply this gives us a bit of head room for board, lights etc.)
  FastLED.addLeds<WS2812B, _ledDOutPin, GRB>(leds, _ledNum).setCorrection( TypicalSMD5050 );  //TypicalLEDStrip
  //FastLED.setBrightness(CRGB(255,255,255));         //set RGB individual brightness
  FastLED.setBrightness(_ledGlobalBrightness);      //set global brightness
  FastLED.setTemperature(UncorrectedTemperature);   //set first temperature

}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(_ledNum) ] += CRGB::White;
  }
}


