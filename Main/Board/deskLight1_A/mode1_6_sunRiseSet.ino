/*----------------------------mode1 sunrise and mode6 sunset----------------------------*/
/*
 * sunrise and sunset modes (1 & 6) are usually kept out of the preset lists and just triggered by the alarm interrupts
 * ..but it is a mode and can be selected as such 
 * eg. for testing/demo porpoises
 */
void mode1() {
  
  //name should be sunrise!
  //isStatic default is false
  //if triggered by an alarm, then temporarily make sure isStatic is set to false
  if(modeInfo[1].isStatic) {
    //show hyperthetical mid-point levels
  }
  else {
    sunRise();
    addGlitter(80);
  }
  
} //END mode1


void mode6() {
  
  //name should be sunset!
  //isStatic default is false
  if(modeInfo[6].isStatic) {
    //
  }
  else {
    sunSet();
  }
  
} //END mode6

/*----------------------------set----------------------------*/
void setSunRise(int mode, uint8_t hour, uint8_t mins) {
  setSunRiseSet(false, mode, hour, mins);
}

void setSunSet(int mode, uint8_t hour, uint8_t mins) {
  setSunRiseSet(true, mode, hour, mins);
}

void setSunRiseSet(boolean riseSet, int mode, uint8_t hour, uint8_t mins) {
  //mode 0 = natural (set rise to realtime) - Not used yet!
  //mode 1 = set time aswell (mabye link to alarm array entry)
  //RTC.disableAlarms();                      //hmm.. power cuts? ..and how to specify 1 or both? write my own?
  DateTime MyTimestamp;
  MyTimestamp = RTC.read();                 //get an initialized timestamp to use
  if(mode == 0) {
    //read table from somewhere? is AT24C32 storage big enough?
    //sat 17 dec 16 nautical twilight 17:12
    MyTimestamp.Hour = 17;
    MyTimestamp.Minute = 12;
    MyTimestamp.Second = 0;
  } else if(mode == 1) {
    MyTimestamp.Hour = hour;
    MyTimestamp.Minute = mins;
    MyTimestamp.Second = 0;
  }
  if(riseSet) {
    RTC.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_SECOND_MINUTE_HOUR);   //sunRise - alarm2
  } else {
    RTC.setAlarm(MyTimestamp, DS3231_Simple::ALARM_DAILY);                      //sunSet - alarm1
  }
}

/*----------------------------do----------------------------*/
/*
 * sunrise/set states can be used by such as static/moving
 * eg. static could go straight to the end colours, or the frozen midpoint of the 'do' state
 * 
 * //setup
 * beginning
 * do rise
 * end
 */
 //TOSO - need to wrap-up rise/set into one call !!!
void sunRise() {
  //time to rise (ms)
  //start/end levels
  //number of steps (LED total)
  //time interval for each step (ms)
  //levels for each step
  
  if(_sunRiseStateCur == 0) { /* begin */
    fadeToBlackBy( leds, _ledNum, 30);                        //fade to black to setup (sunset will need a fade-up to whatever the starts levels are)
    //now show first step of sequence
    
    //wait for the appropriate divison of time
    _sunRiseStateCur = 1; //set next state
  }
  else if(_sunRiseStateCur == 1) { /* do */
    //do/show rest of sequence (except the last step)

    //when finished..
    _sunRiseStateCur = 2;
  }
  else if(_sunRiseStateCur == 2) { /* end */
    //show last step of sequence
    
    //wait for the appropriate divison of time
    _modePresetSlotCur += 1;                                  //TEMP - next mode
    _sunRiseStateCur = 0;                                     //reset for next time
  }
  
    int startPx0 = (0 - _segmentTotal);
    int endPx0 = 0;
    //map(int hour, 0, 12, _segmentStart[0], _segmentEnd[_segmentTotal-1]);
    
    for(int i=ledSegment[0].first; i<ledSegment[0].last; i++) {
      leds[i] = CRGB(255, 255, 255);
    }
    for(int i=ledSegment[2].first; i<ledSegment[2].last; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    
    fill_gradient_RGB(leds, ledSegment[1].first, CRGB(255, 255, 255), ledSegment[1].last, CRGB(0, 0, 0));
    fill_gradient_RGB(leds, ledSegment[3].first, CRGB(0, 0, 0), ledSegment[3].last, CRGB(255, 255, 255) );

    //when sunrise is finished either stay bright, or change(fade) to previous levels
    //set tomorrows sunrise time
    //_sunRiseEnabled = false;                //reset
    //set next mode as morning or day
  
}

void sunSet() {
  //if(_sunSetEnabled) {
    
    //if lights are currently off..
    //save state,
    //then fade eveything to start levels
    //..now do sunset
    //
    //when sunset is finished either stay bright, or change(fade) to previous levels
    //set tomorrows sunset time
    //_sunSetEnabled = false; //switch everything off and lock the doors behind you, ..until next time
    
  //}
}


