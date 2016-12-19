/*----------------------------interrupt----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(DS3231_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DS3231_INTERRUPT_PIN), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
  if(_firstTimeSetupDone) {         //hmm.. really don't think i should rely on this var
    _sunRiseSetTriggered = true;
  }
  DEBUG_PRINT("interrupts - sunRiseSetInterrupt triggered")
}


