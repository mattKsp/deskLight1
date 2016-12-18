/*----------------------------interrupts----------------------------*/
//attachInterrupt(interruptPin, function, CHANGE);

void setupInterrupts() {
  pinMode(DS3231_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DS3231_INTERRUPT_PIN), sunRiseSetInterrupt, FALLING);  //FALLING, RISING, CHANGE
}

void sunRiseSetInterrupt() {
//  //triggered by sunRise/alarm2 & sunSet/alarm1
//  DateTime MyDateAndTime;
//  MyDateAndTime = RTC.read();
//
//  if(MyDateAndTime.Hour < 9) {  //TEMP 9, should be 12
//    //is time is before 12:00
//    //start sunrise sequence
//    _sunRiseTriggered = true;
//  } else {
//    //else if time is 12:00 or after
//    //start sunset sequence
//    _sunSetTriggered = true;
//  }
  //RTC.printTo(Serial); Serial.println(": sunRiseSetInterrupt triggered");
  if(_firstTimeSetupDone) {
    _sunRiseSetTriggered = true;
  }
}


