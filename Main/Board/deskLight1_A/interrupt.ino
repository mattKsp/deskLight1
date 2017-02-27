/*----------------------------interrupt----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

//void setupInterrupts() {
//  pinMode(_DS3231interruptPin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(_DS3231interruptPin), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
//}
//
//void sunRiseSetInterrupt() {
//  //_sunRiseSetTriggered = true;
//  //..keep interrupts short, yes i know, but in this case there is nothing time sensitive, no other interrupts etc.,
//  //in short, i am not concerned over the length of the following function
//  //sunRiseSetCheck();
//  if(_firstTimeSetupDone) {
//    _modeCur == 1;
//    _sunRiseStateCur = 0;
//    _onOff = true;
//  }
//  #ifdef DEBUG
//    Serial.print(F("interrupt triggered"));
//    Serial.println();
//    blinkStatusLED();
//  #endif
//}


