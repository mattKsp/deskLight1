/*----------------------------leds----------------------------*/

void setupLEDs() {
  
  //system LEDs
  pinMode(LED_PIN, OUTPUT); //Setup the internal LED on pin 13
  digitalWrite(LED_PIN, _ledState);

    //FastLED
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 2850);  //limit power draw to 2.85A at 5v (with 3A power supply this gives us a bit of head room for board, lights etc.)
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );  //TypicalLEDStrip
    //FastLED.setBrightness(CRGB(255,255,255));  //set RGB individual brightness
    FastLED.setBrightness(BRIGHTNESS);            //set global brightness
    FastLED.setTemperature( UncorrectedTemperature ); //set first temperature

//can i do this here?  ..do i need to?
    //FastLED.show(); //send all the data to the strips
    //FastLED.delay(8);
}



