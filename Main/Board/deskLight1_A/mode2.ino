/*----------------------------mode2----------------------------*/

//called from main loop
void mode2() {
  
  if(_mode2_sub == 0) { mode2_sub0(); }
  else if(_mode2_sub == 1) { mode2_sub1(); }
  else if(_mode2_sub == 2) { mode2_sub2(); }
  else if(_mode2_sub == 3) { mode2_sub3(); }

} //end mode2

void mode2_sub0() {
  //gradient - start color to end color
    
    for(int i=_segmentStart[0]; i<_segmentEnd[0]; i++) {
      leds[i] = startColor_RGB;
    }
  
    fill_gradient_RGB(leds, _segmentStart[1], startColor_RGB, _segmentEnd[1], endColor_RGB);
    
    for(int i=_segmentStart[2]; i<_segmentEnd[2]; i++) {
      leds[i] = endColor_RGB;
    }
    
    fill_gradient_RGB(leds, _segmentStart[3], endColor_RGB, _segmentEnd[3], startColor_RGB );  //end93/149
}

void mode2_sub1() {
  //gradient of white from bottom to top
    
  for(int i=_segmentStart[0]; i<_segmentEnd[0]; i++) {
    leds[i] = CRGB(255, 255, 255);
  }

  fill_gradient_RGB(leds, _segmentStart[1], CRGB(255, 255, 255), _segmentEnd[1], CRGB(0, 0, 0));
  
  for(int i=_segmentStart[2]; i<_segmentEnd[2]; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  
  fill_gradient_RGB(leds, _segmentStart[3], CRGB(0, 0, 0), _segmentEnd[3], CRGB(255, 255, 255) );
}

void mode2_sub2() {
  //gradient of white from top to bottom
    
  for(int i=_segmentStart[0]; i<_segmentEnd[0]; i++) {
    leds[i] = CRGB(0, 0, 0);
  }

  fill_gradient_RGB(leds, _segmentStart[1], CRGB(0, 0, 0), _segmentEnd[1], CRGB(255, 255, 255));
  
  for(int i=_segmentStart[2]; i<_segmentEnd[2]; i++) {
    leds[i] = CRGB(255, 255, 255);
  }
  
  fill_gradient_RGB(leds, _segmentStart[3], CRGB(255, 255, 255), _segmentEnd[3], CRGB(0, 0, 0) );
}

void mode2_sub3() {
  //full white
  fill_solid( leds, LED_NUM, CRGB(255, 255, 255)); 
}


  
