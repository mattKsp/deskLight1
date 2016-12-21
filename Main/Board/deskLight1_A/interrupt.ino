/*----------------------------interrupt----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(_DS3231interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_DS3231interruptPin), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
  //_sunRiseSetTriggered = true;
  sunRiseSetCheck();
  DEBUG_PRINT("interrupts - sunRiseSetInterrupt triggered")
}

void sunRiseSetCheck() {
  if(_firstTimeSetupDone) {
    DateTime curDT;
    curDT = RTC.read();
    if(curDT.Hour > 12) {
      //_sunSetEnabled = true;
      _modeCur == 6;
      modeInfo[6].isStatic = false;
      _sunSetStateCur = 0;
    } else {
      //_sunRiseEnabled = true;
      _modeCur == 1;
      modeInfo[1].isStatic = false;
      _sunRiseStateCur = 0;
    }
    //_sunRiseSetTriggered = false;
    DEBUG_PRINT("sunRiseSetCheck - sunRiseSetInterrupt triggered")
  }
}


