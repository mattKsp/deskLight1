/*----------------------------mode1----------------------------*/

//called from main loop
void mode1() {
  
  // draw a generic, no-name rainbow
  //static uint8_t starthue = 0;
  //fill_rainbow (struct CRGB *pFirstLED, int numToFill, uint8_t initialhue, uint8_t deltahue=5)
  //fill_rainbow( leds + 5, LED_NUM - 5, --starthue, 20);

  //fill_solid( leds, LED_NUM, CRGB(50,0,200)); 
  //fill_solid( leds, LED_NUM, CRGB(50,0,200));
  //fill_gradient(T *targetArray, uint16_t startpos, CHSV startcolor, uint16_t endpos, CHSV endcolor, TGradientDirectionCode directionCode=SHORTEST_HUES)
  //fill_gradient (T *targetArray, uint16_t numLeds, const CHSV &c1, const CHSV &c2, TGradientDirectionCode directionCode=SHORTEST_HUES)  // ???
  //fill_gradient( leds, 0,  );
  //FORWARD_HUES: hue always goes clockwise
  //BACKWARD_HUES: hue always goes counter-clockwise
  //SHORTEST_HUES: hue goes whichever way is shortest
  //LONGEST_HUES: hue goes whichever way is longest
  //fill_gradient(ledSideA,0,CHSV(0,255,255),64,CHSV(0,255,255),SHORTEST_HUES); 
  //fill_gradient(ledSideB,0,CHSV(0,255,255),64,CHSV(0,255,255),SHORTEST_HUES);
  
  //fill_gradient(leds, 10, startColor, 74, endColor, SHORTEST_HUES); //side A
  //fill_gradient(leds, 85, endColor, 149, startColor, SHORTEST_HUES);  //side B (colors reversed, because the numbers are running top to bottom due to length of wiring considerations..

  //if(_mode1_sub == 0) {
    for(int i=_segmentStart[0]; i<_segmentEnd[0]; i++) {
      leds[i] = startColor_RGB;
    }
  
    fill_gradient_RGB(leds, _segmentStart[1], startColor_RGB, _segmentEnd[1], endColor_RGB);
    
    for(int i=_segmentStart[2]; i<_segmentEnd[2]; i++) {
      leds[i] = endColor_RGB;
    }
    
    fill_gradient_RGB(leds, _segmentStart[3], endColor_RGB, _segmentEnd[3], startColor_RGB );  //end93/149
  //}
  //leds.fill_gradient(10, startColor, 74, endColor, SHORTEST_HUES);  //with RGBArray ..which i cant get no worky

  //FastLED.setTemperature( UncorrectedTemperature ); //set first temperature  //..moved to setup ..might work ???
  //leds[0] = TEMPERATURE_1; //show indicator pixel

  //
  //checkSegmentEndpoints();
} //end mode1


