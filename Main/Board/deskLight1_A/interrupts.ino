/*----------------------------interrupts----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(DS3231_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DS3231_INTERRUPT_PIN), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
  if(_firstTimeSetupDone) {
    _sunRiseSetTriggered = true;
  }
//  #ifdef DEBUG
//  RTC.printTo(Serial); 
//  Serial.println(": sunRiseSetInterrupt triggered");
//  #endif
  DEBUG_PRINT("interrupts - sunRiseSetInterrupt triggered")
}


