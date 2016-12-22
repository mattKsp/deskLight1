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
  sunRiseSetCheck();
  DEBUG_PRINT("interrupts - sunRiseSetInterrupt triggered") //this will not be compiled if DEBUG is not defined
  #ifdef DEBUG
    //also.. this will not be compiled if DEBUG is not defined
    blinkStatusLED();
  #endif
}

void sunRiseSetCheck() {
  if(_firstTimeSetupDone) {
    DateTime curDT;
    curDT = RTC.read();
    if(curDT.Hour > 12) {
      //_sunSetEnabled = true;
      _modeCur == 6;
      modeInfo[6].isStatic = false;
//TEMP - testing
//      _modeCur == 1;
//      modeInfo[1].isStatic = false;
//      _sunRiseStateCur = 0;
      _onOff = true;
    } else {
      //_sunRiseEnabled = true;
      _modeCur == 1;
      modeInfo[1].isStatic = false;
      _sunRiseStateCur = 0;
      _onOff = true;
    }
    //_sunRiseSetTriggered = false;
    DEBUG_PRINT("sunRiseSetCheck - sunRiseSetInterrupt triggered")
  }
}


