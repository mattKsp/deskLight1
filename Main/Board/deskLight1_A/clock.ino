/*----------------------------clock----------------------------*/
//http://forum.arduino.cc/index.php?topic=260032.0
//http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/
//https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
//
//byte decToBcd(byte val) {
////convert normal decimal numbers to binary coded decimal
//  return( (val/10*16) + (val%10) );
//}
//byte bcdToDec(byte val) {
////convert binary coded decimal to normal decimal numbers
//  return( (val/16*10) + (val%16) );
//}
//int decToInt(byte val) {
//  //convert binary coded decimal to int
//  return( int(val) );
//}
//
///*------------------DS3231-----------------*/
//// definition for register addresses
//#define DS3231_ALARM1_SEC 0x07 //alarm 1 seconds
//#define DS3231_ALARM1_MINS 0x08 //alarm 1 mins
//#define DS3231_ALARM1_HOUR 0x09 //alarm 1 hour
//#define DS3231_ALARM1_DAYDATE 0x0A //alarm 1 day/date
//#define DS3231_ALARM2_MINS 0x0B //alarm 2 mins
//#define DS3231_ALARM2_HOUR 0x0C //alarm 2 hour
//#define DS3231_ALARM2_DAYDATE 0x0D //alarm 2 day/date
//#define DS3231_CONTROL 0x0E //control
//#define DS3231_STATUS 0x0F //control/status
//
//void DS3231setTime(byte sec, byte mins, byte hr, byte day, byte date, byte month, byte year) {
//  Wire.beginTransmission(DS3231_I2C_ADDRESS);
//  Wire.write(0);
//  Wire.write(decToBcd(sec));
//  Wire.write(decToBcd(mins));
//  Wire.write(decToBcd(hr));
//  Wire.write(decToBcd(day));
//  Wire.write(decToBcd(date));
//  Wire.write(decToBcd(month));
//  Wire.write(decToBcd(year));
//  Wire.endTransmission();
//}
//
//void DS3231getTime(byte *sec, byte *mins, byte *hr, byte *day, byte *date, byte *month, byte *year) {
//  Wire.beginTransmission(DS3231_I2C_ADDRESS);
//  Wire.write(0);  //set the (next) register to point at 00h
//  Wire.endTransmission();
//  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);  //request 7 bytes of data from the DS3231, start at register 00h
//  *sec = bcdToDec(Wire.read() & 0x7f);
//  *mins = bcdToDec(Wire.read());
//  *hr = bcdToDec(Wire.read() & 0x3f);
//  *day = bcdToDec(Wire.read());
//  *date = bcdToDec(Wire.read());
//  *month = bcdToDec(Wire.read());
//  *year = bcdToDec(Wire.read());
//}
//
//void DS3231write(byte loc, byte data) {
//  Wire.beginTransmission(DS3231_I2C_ADDRESS);
//  Wire.write(loc);  //location
//  Wire.write(data);
//  Wire.endTransmission();
//}
//
//byte DS3231read(byte location) {
// byte result;
// Wire.beginTransmission(DS3231_I2C_ADDRESS);
// Wire.write(loc); //location
// Wire.endTransmission();  
// Wire.requestFrom(DS3231_I2C_ADDRESS, 1);//1 byte
// result = Wire.read();
// return result;
//}
//
//void DS3231setAlarm1(byte sec, byte mins, byte hr, byte day, byte date, byte month, byte year) {
////
////#define DS3231_ALARM1_SEC 0x07 //A1M1 - alarm 1 seconds
////#define DS3231_ALARM1_MINS 0x08 //A1M2 - alarm 1 mins
////#define DS3231_ALARM1_HOUR 0x09 //A1M3 - alarm 1 hour
////#define DS3231_ALARM1_DAYDATE 0x0A //A1M4 -alarm 1 day/date
////#define DS3231_ALARM2_MINS 0x0B //A2M2 - alarm 2 mins
////#define DS3231_ALARM2_HOUR 0x0C //A2M3 - alarm 2 hour
////#define DS3231_ALARM2_DAYDATE 0x0D //A2M4 - alarm 2 day/date
////#define DS3231_CONTROL 0x0E //control
////#define DS3231_STATUS 0x0F //control/status
////  
//DS3231write(DS3231_ALARM1_SEC, B10000000);
//DS3231write(DS3231_ALARM1_MINS, B10000000);
//DS3231write(DS3231_ALARM1_HOUR, B10000000);
//DS3231write(DS3231_CONTROL, B00000111); //enable interrupt pin, alarm 2 and alarm 1
//  
//}

/*------------------LED overlays-----------------*/
//these will probably be overlaid onto whatever is showing at the time

const int _showTimeUpdateInterval = 1000; //1 sec = 1000 milliseconds
unsigned long _previousMillis = 0;

void showTime() {
  //need actual realtime clock to be able to do this, so fake it to try
  //time in 12 hour format
  //..got one..
//  byte sec, mins, hr, day, date, month, year; //set empty bytes
//  DS3231getTime(&sec, &mins, &hr, &day, &date, &month, &year);  //get results from the RTC and copy onto the empty bytes
  //use bytes..
  //int _hour = 3;
  //int _minute = 25;
  //int i = map(decToInt(hr), 0, 12, _segmentEnd[_segmentTotal-1], _segmentStart[0] );
  //int j = map(decToInt(mins), 0, 60, _segmentStart[0], _segmentEnd[_segmentTotal-1]);
  unsigned long currentMillis = millis(); //save state
  if((unsigned long)(currentMillis - _previousMillis) >=  _showTimeUpdateInterval) {
    DateTime MyDateAndTime;
    MyDateAndTime = RTC.read();
    
    int i = map(MyDateAndTime.Hour, 0, 12, _segmentEnd[_segmentTotal-1], _segmentStart[0] );
    int j = map(MyDateAndTime.Minute, 0, 60, _segmentEnd[_segmentTotal-1], _segmentStart[0] );
    int k = map(MyDateAndTime.Second, 0, 60, _segmentEnd[_segmentTotal-1], _segmentStart[0] );
    leds[i] = -leds[i]; //invert
    leds[j] = -leds[j]; //invert
    leds[k] = -leds[k]; //invert
    leds[i] += CRGB( 20, 0, 0); //add a little red
    leds[j] += CRGB( 0, 20, 0); //add a little green
    leds[k] += CRGB( 0, 0, 20); //add a little blue

    #ifdef DEBUG
      Serial.print(MyDateAndTime.Hour);
      Serial.print(":");
      Serial.print(MyDateAndTime.Minute);
      Serial.print(".");
      Serial.print(MyDateAndTime.Second);
      Serial.print(" - ");
      Serial.print(MyDateAndTime.Minute);
      Serial.print("/");
      Serial.print(MyDateAndTime.Day);
      Serial.print("/");
      Serial.print(MyDateAndTime.Month);
      Serial.print("/");
      Serial.print(MyDateAndTime.Year);
      Serial.println(" ");
    #endif
    
    _previousMillis = currentMillis;  //save state for next time around
  }
}

void setSunRise(int mode, uint8_t hour, uint8_t mins) {
  //mode 0 = natural (set rise to realtime)
  //mode 1 = set time aswell (mabye link to alarm array entry)
  //uses alarm 2
  //RTC.disableAlarms();  //hmm.. power cuts? ..and how to specify 1 or both?
  DateTime MyTimestamp;
  MyTimestamp = RTC.read();  //get an initialized timestamp to use
  if(mode == 0) {
    //read table from somewhere? is AT24C32 storage big enough?
    //sun 18 dec 16 nautical twilight 06:49
    MyTimestamp.Hour = 6;
    MyTimestamp.Minute = 49;
  } else if(mode == 1) {
    MyTimestamp.Hour = hour;
    MyTimestamp.Minute = mins;
  }
  RTC.setAlarm(MyTimestamp, DS3231_Simple::ALARM_DAILY);
}

void setSunSet(int mode, uint8_t hour, uint8_t mins) {
  //mode 0 = natural (set fall to realtime)
  //mode 1 = set time (mabye link to alarm array entry)
  //uses alarm 1 - might have to be set after sunrise trigger everyday
  //RTC.disableAlarms();  //hmm.. power cuts? ..and how to specify 1 or both?
  DateTime MyTimestamp = RTC.read();  //get an initialized timestamp to use
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
  RTC.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_SECOND_MINUTE_HOUR);
}

void sunRiseSetCheck() {
  //mabye restrict time check interval ?
  if(_sunRiseSetTriggered) {
    DateTime curDT;
    curDT = RTC.read();
    if(curDT.Hour > 12) {
      _sunSetEnabled = true;
    } else {
      _sunRiseEnabled = true;
    }
    _sunRiseSetTriggered = false;
    
//    #ifdef DEBUG
//      Serial.println("sunRiseSetInterrupt triggered");
//    #endif
    DEBUG_PRINT("sunRiseSetCheck - sunRiseSetInterrupt triggered")
  }
}

void sunRise() {
//  if(_sunRiseSetTriggered) {
//    DateTime MyDateAndTime;
//    MyDateAndTime = RTC.read();
//    if(MyDateAndTime.Hour < 12) {
//      _sunRiseEnabled = true;
//      _sunRiseSetTriggered = false;
//    }
//  }
//  if(MyDateAndTime.Hour < 9) {  //TEMP 9, should be 12
//    //is time is before 12:00
//    //start sunrise sequence
//    _sunRiseTriggered = true;
//  } else {
//    //else if time is 12:00 or after
//    //start sunset sequence
//    _sunSetTriggered = true;
//  }
//  if(_sunRiseTriggered) {
//    _sunRiseEnabled = true;
//    _sunRiseTriggered = false;  //reset for next interrupt
//  }
  if(_sunRiseEnabled) {
    //if lights are currently on..
    //save state,
    //then fade everything to black
    //..now do sun rise
    
    int startPx0 = (0 - _segmentTotal);
    int endPx0 = 0;
    //map(int hour, 0, 12, _segmentStart[0], _segmentEnd[_segmentTotal-1]);
    
    for(int i=_segmentStart[0]; i<_segmentEnd[0]; i++) {
      leds[i] = CRGB(255, 255, 255);
    }
    for(int i=_segmentStart[2]; i<_segmentEnd[2]; i++) {
      leds[i] = CRGB(0, 0, 0);
    }
    
    fill_gradient_RGB(leds, _segmentStart[1], CRGB(255, 255, 255), _segmentEnd[1], CRGB(0, 0, 0));
    fill_gradient_RGB(leds, _segmentStart[3], CRGB(0, 0, 0), _segmentEnd[3], CRGB(255, 255, 255) );

    //when sunrise is finished either stay bright, or change(fade) to previous levels
    //set tomorrows sunrise time
    //_sunRiseEnabled = false;  //reset
  }
}

void sunSet() {
  if(_sunSetEnabled) {
    
    //if lights are currently off..
    //save state,
    //then fade eveything to start levels
    //..now do sunset
    //
    //when sunset is finished either stay bright, or change(fade) to previous levels
    //set tomorrows sunset time
    //_sunSetEnabled = false; //switch everything off and lock the doors behind you, ..until next time

    #ifdef DEBUG
      DateTime MyDateAndTime;
      MyDateAndTime = RTC.read();
      Serial.print("sunset enabled");
      Serial.print(" @ ");
      Serial.print(MyDateAndTime.Hour);
      Serial.print(":");
      Serial.print(MyDateAndTime.Minute);
      Serial.println();
    #endif
    
  }
}

