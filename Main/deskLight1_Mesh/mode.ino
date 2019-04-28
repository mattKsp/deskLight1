/*----------------------------mode----------------------------*/

/*
 * 0 - glow     (static/moving)
 * 1 - sunrise  (static/moving)
 * 2 - morning  (static/moving)
 * 3 - day      (static/moving) (full bright/overcast/clearbluesky)
 * 4 - working  (static) (full bright) (with colour temperature sub-mode)
 * 5 - evening  (static/moving)
 * 6 - sunset   (static/moving)
 * 7 - night    (static/moving)
 * 8 - effect   (moving) (could use static to pause it?)
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
    loopBreathing();
  }

}

/*----------------------------modes----------------------------*/
void mode0() {
  // name should be glow!
  // default is static (mabye with a few sparkles)
  for(int i = ledSegment[1].first; i <= ledSegment[1].last; i++) {
    strip.SetPixelColor(i, _hslGlowStart);
  }
  FillGradientRGB(ledSegment[2].first, ledSegment[2].total, _hslGlowStart, _hslGlowEnd);
  for(int i = ledSegment[3].first; i <= ledSegment[3].last; i++) {
    strip.SetPixelColor(i, _hslGlowEnd);
  }
  FillGradientRGB(ledSegment[4].first, ledSegment[4].total, _hslGlowEnd, _hslGlowStart);
    
}

//

void mode2() {
  // name should be morning!
  for(int i = ledSegment[1].first; i <= ledSegment[1].last; i++) {
    strip.SetPixelColor(i, _hslMorningStart);
  }
  FillGradientRGB(ledSegment[2].first, ledSegment[2].total, _hslMorningStart, _hslMorningEnd);
  for(int i = ledSegment[3].first; i <= ledSegment[3].last; i++) {
    strip.SetPixelColor(i, _hslMorningEnd);
  }
  FillGradientRGB(ledSegment[4].first, ledSegment[4].total, _rgbBlack, _hslMorningStart);
}

void mode3() {
  // name should be day!
  for(int i = ledSegment[1].first; i <= ledSegment[1].last; i++) {
    strip.SetPixelColor(i, _hslDayStart);
  }
  FillGradientRGB(ledSegment[2].first, ledSegment[2].total, _hslDayStart, _hslDayEnd);
  for(int i = ledSegment[3].first; i <= ledSegment[3].last; i++) {
    strip.SetPixelColor(i, _hslDayEnd);
  }
  FillGradientRGB(ledSegment[4].first, ledSegment[4].total, _hslDayEnd, _hslDayStart);
}

void mode4() {
  // name should be working!
  for (int i = ledSegment[1].first; i <= ledSegment[4].last; i++) {
    strip.SetPixelColor(i, _rgbColorTempCur);
  }
}

void mode5() {
  // name should be evening!
  for(int i = ledSegment[1].first; i <= ledSegment[4].last; i++) {
    strip.SetPixelColor(i, _hslEveningStart);
  }
}

//

void mode7() {
  // name should be night!
   for(int i = ledSegment[1].first; i <= ledSegment[1].last; i++) {
      strip.SetPixelColor(i, _hslNightStart);
    }
    FillGradientRGB(ledSegment[2].first, ledSegment[2].total, _hslNightStart, _hslNightEnd);
    for(int i = ledSegment[3].first; i <= ledSegment[3].last; i++) {
      strip.SetPixelColor(i, _hslNightEnd);
    }
    FillGradientRGB(ledSegment[4].first, ledSegment[4].total, _hslNightEnd, _hslNightStart);
}

void mode8() {
  // name should be effect!
  
  // TEMP
  for (int i = ledSegment[1].first; i <= ledSegment[4].last; i++) {
    strip.SetPixelColor(i, _hslEffect0);
  }
  
}


/*----------------------------Breathing----------------------------*/

/*
 * Breath : (noun) Refers to a full cycle of breathing. It can also refer to the air that is inhaled or exhaled.
 */
 
// would eventually like a simple timeline screen on an app where you can draw curves in and see the results in realtime
//const unsigned long _breathRiseFallStepIntervalMillis = 250;  //156  62  139  2 //breath rise/fall step interval in milliseconds
//const int _breathRiseFallSpacer = 4;  //just 4 at begin.. eg. 4 each end.. //8   //_breathMaxBrightness / (4*2)     //counts to pause/ignore at the bottom and top - 42
//const int _breathMaxBrightness = 16;  //32
//const unsigned long _breathRiseFallHoldIntervalMillis = 1;    //breath rise/fall hold interval in milliseconds
//unsigned long _breathRiseFallPrevMillis = 0;                  //previous time for reference
//int _breathRiseFallCounter = 0;                               //eg. 0-17
//boolean _breathRiseFallDirection = true;                      //direction true-rise, false-fall
//CRGB c;
//RgbColor c;

void loopBreathing() {
  if (_isBreathing == true) {
    breathRiseFall2();
  } else {
    FadeAll(4);
  }
}

uint8_t bBpm = 12;
uint8_t bMax = ( (ledSegment[2].total / 4) - 1 );
uint8_t bPeak;

void breathRiseFall2() {

//  if (_isBreathOverlaid == false) {
    strip.ClearTo(_rgbBlack);
//  }
  
  bPeak = beatsin8( bBpm, 1, bMax); //bpm, min, max
  
  //fade bot to top
  //ledsLeft( 1, bPeak ).fill_gradient_RGB( CRGB::White, CRGB::Black );

  //strip.SetPixelColor(i, _rgbWhite);
  
  FillGradientRGB(ledSegment[2].first, bPeak, _rgbWhite, _rgbBlack);  // left
  FillGradientRGB((ledSegment[4].first+bPeak), ledSegment[4].total, _rgbBlack, _rgbWhite);  // right
  
}
