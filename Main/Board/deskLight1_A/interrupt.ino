/*----------------------------interrupt----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(_DS3231interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_DS3231interruptPin), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
  //_sunRiseSetTriggered = true;
  //..keep interrupts short, yes i know, but in this case there is nothing time sensitive, no other interrupts etc.,
  //in short, i am not concerned over the length of the following function
  //sunRiseSetCheck();
  #ifdef DEBUG
    Serial.print(F("interrupt triggered"));
    Serial.println();
    blinkStatusLED();
  #endif
}
/*
 * PROBLEM - whenever we read from the chip, it resets both the alarms!
 * solution - scrap having both sunrise and sunset, use alarm 2 to trigger each day, and alarm 1 for the rise increments, then reset for next day
 * code getting too heavy, strip it out and mabye put in sunset later..
 */
void sunRiseSetCheck() {
  if(_firstTimeSetupDone) {
    DateTime curDT;
    curDT = RTC.read();
    if(curDT.Hour > 12) {
      //_sunSetEnabled = true;
//      _modeCur == 6;
//      modeInfo[6].isStatic = false;
//TEMP - testing
      _modeCur == 1;
      modeInfo[1].isStatic = false;
      _sunRiseStateCur = 0;
      _onOff = true;
    } else {
      //_sunRiseEnabled = true;
      _modeCur == 1;
      modeInfo[1].isStatic = false;
      _sunRiseStateCur = 0;
      _onOff = true;
    }
    //_sunRiseSetTriggered = false;
  }
}


