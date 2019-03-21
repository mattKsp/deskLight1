/*----------------------------mode----------------------------*/

/*
 * 0 - glow     (static/moving)
 * 1 - sunrise  (static/moving)
 * 2 - morning  (static/moving)
 * 2 - day      (static/moving) (full bright/overcast/clearbluesky)
 * 4 - working  (static) (full bright) (colour temperature sub-mode)
 * 5 - evening  (static/moving)
 * 6 - sunset   (static/moving)
 * 7 - night    (static/moving)
 * 8 - changing (moving) (could use static to pause it?)
 * 
 * gradient fade (top/mid/bot) (all can invert) (overlay/fx?)
 * moveable/resizable bright bar (overlay/fx?) (might use brightness +/- intputs to move up/down)
 */

/*
  select mode to run based upon mode var, set by input
  called from main loop
  (select from preset array) ..later
  numbers denote slots
  slots are filled (by hand for now) with named functions
  ..needs an override aswell
  modes are represented as numbers, but have names if needed
  modes are selected individualy or in a present sequence

 */
void loopModes() {
  if (_onOff) {
    if(_modeCur == 0) {       mode0(); } 
    else if(_modeCur == 1) {  mode1(); }
    else if(_modeCur == 2) {  mode2(); }
    else if(_modeCur == 3) {  mode3(); }
    else if(_modeCur == 4) {  mode4(); }
    else if(_modeCur == 5) {  mode5(); }
    else if(_modeCur == 6) {  mode6(); }
    else if(_modeCur == 7) {  mode7(); }
    else if(_modeCur == 8) {  mode8(); }
  } 
  else { 
    //fadeToBlackBy( leds, _ledNum, 30); 
    fill_solid( leds, _ledNum, CRGB::Black);  //TEMP colour ..this should be a number, not a colour!!!
  } //turn off - fade everything to black
}


/*----------------------------modes----------------------------*/
void mode0() {
  //name should be glow!
  //default is static (with a few sparkles)
    for(int i = ledSegment[0].first; i <= ledSegment[0].last; i++) {
      leds[i] = startColor_RGB;
    }
    fill_gradient_RGB(leds, ledSegment[1].first, CRGB(32, 32, 32), ledSegment[1].last, CRGB::Black);
    for(int i = ledSegment[2].first; i <= ledSegment[2].last; i++) {
      leds[i] = CRGB::Black;
    }
    fill_gradient_RGB(leds, ledSegment[3].first, CRGB::Black, ledSegment[3].last, CRGB(32, 32, 32) );
    
    //addGlitter(80);
}

//

void mode2() {
  //name should be morning!
    for(int i = ledSegment[0].first; i <= ledSegment[0].last; i++) {
      leds[i] = CRGB::Yellow;
    }
    fill_gradient_RGB(leds, ledSegment[1].first, CRGB::Yellow, ledSegment[1].last, CRGB::Black);
    for(int i = ledSegment[2].first; i <= ledSegment[2].last; i++) {
      leds[i] = CRGB::Black;
    }
    fill_gradient_RGB(leds, ledSegment[3].first, CRGB::Black, ledSegment[3].last, CRGB::Yellow );
}

void mode3() {
  //name should be day!
    for(int i = ledSegment[0].first; i <= ledSegment[0].last; i++) {
      leds[i] = CRGB::Green;
    }
    fill_gradient_RGB(leds, ledSegment[1].first, CRGB::Green, ledSegment[1].last, CRGB::Yellow);
    for(int i = ledSegment[2].first; i <= ledSegment[2].last; i++) {
      leds[i] = CRGB::Yellow;
    }
    fill_gradient_RGB(leds, ledSegment[3].first, CRGB::Yellow, ledSegment[3].last, CRGB::Green );
}

void mode4() {
  //name should be working!
    fill_solid( leds, _ledNum, CRGB::White);  //TEMP colour
/*
 //sub temperature modes later..
  if(_mode0_sub == 0) {
    FastLED.setTemperature( TEMPERATURE_0 ); //set first temperature  //..moved to setup ..might work ???
    //fill_solid( leds, _ledNum, TEMPERATURE_0 );
    leds[0] = TEMPERATURE_0; //show indicator pixel
  } else if(_mode0_sub == 1) {
    FastLED.setTemperature( TEMPERATURE_1 );
    //fill_solid( leds, _ledNum, TEMPERATURE_1 );
    leds[0] = TEMPERATURE_1;
  } else if(_mode0_sub == 2) {
    FastLED.setTemperature( TEMPERATURE_2 );
    //fill_solid( leds, _ledNum, TEMPERATURE_2 );
    leds[0] = TEMPERATURE_3;
  } else if(_mode0_sub == 3) {
    FastLED.setTemperature( TEMPERATURE_3 );
    //fill_solid( leds, _ledNum, TEMPERATURE_3 );
    leds[0] = TEMPERATURE_3;
  } else if(_mode0_sub == 4) {
    FastLED.setTemperature( TEMPERATURE_4 );
    //fill_solid( leds, _ledNum, TEMPERATURE_4 );
    leds[0] = TEMPERATURE_4;
  } else if(_mode0_sub == 5) {
    FastLED.setTemperature( TEMPERATURE_5 );
    //fill_solid( leds, _ledNum, TEMPERATURE_5 );
    leds[0] = TEMPERATURE_5;
  } else if(_mode0_sub == 6) {
    FastLED.setTemperature( TEMPERATURE_6 );
    //fill_solid( leds, _ledNum, TEMPERATURE_6 );
    leds[0] = TEMPERATURE_6;
  } else if(_mode0_sub == 7) {
    FastLED.setTemperature( TEMPERATURE_7 );
    //fill_solid( leds, _ledNum, TEMPERATURE_7 );
    leds[0] = TEMPERATURE_7;
  } else if(_mode0_sub == 8) {
    FastLED.setTemperature( TEMPERATURE_8 );
    //fill_solid( leds, _ledNum, TEMPERATURE_8 );
    leds[0] = TEMPERATURE_8;
  }
  */
}

void mode5() {
  //name should be evening!
    fill_solid( leds, _ledNum, CRGB(128, 64, 64));
}

//

void mode7() {
  //name should be night!
   for(int i = ledSegment[0].first; i <= ledSegment[0].last; i++) {
      leds[i] = CRGB::Black;
    }
    fill_gradient_RGB(leds, ledSegment[1].first, CRGB::Black, ledSegment[1].last, CRGB::White);
    for(int i = ledSegment[2].first; i <= ledSegment[2].last; i++) {
      leds[i] = CRGB::White;
    }
    fill_gradient_RGB(leds, ledSegment[3].first, CRGB::White, ledSegment[3].last, CRGB::Black );
}

void mode8() {
  //name should be changing!
    fill_solid( leds, _ledNum, CRGB(0, 0, 255));
}


