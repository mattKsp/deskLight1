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
//
//const int _showTimeUpdateInterval = 1000;   //1 sec = 1000 milliseconds
//unsigned long _previousMillis = 0;
//
//void showTime() {
//  //need actual realtime clock to be able to do this, so fake it to try
//  //time in 12 hour format
//  //..got one..
////  byte sec, mins, hr, day, date, month, year; //set empty bytes
////  DS3231getTime(&sec, &mins, &hr, &day, &date, &month, &year);  //get results from the RTC and copy onto the empty bytes
//  //use bytes..
//  //int _hour = 3;
//  //int _minute = 25;
//  //int i = map(decToInt(hr), 0, 12, _segmentEnd[_segmentTotal-1], _segmentStart[0] );
//  //int j = map(decToInt(mins), 0, 60, _segmentStart[0], _segmentEnd[_segmentTotal-1]);
//  unsigned long currentMillis = millis();   //save state
//  if((unsigned long)(currentMillis - _previousMillis) >=  _showTimeUpdateInterval) {
//    DateTime MyDateAndTime;
//    MyDateAndTime = RTC.read();
//    
//    int i = map(MyDateAndTime.Hour, 0, 12, ledSegment[_segmentTotal-1].last, ledSegment[0].first );
//    int j = map(MyDateAndTime.Minute, 0, 60, ledSegment[_segmentTotal-1].last, ledSegment[0].first );
//    int k = map(MyDateAndTime.Second, 0, 60, ledSegment[_segmentTotal-1].last, ledSegment[0].first );
//    leds[i] = -leds[i];                     //invert
//    leds[j] = -leds[j];                     //invert
//    leds[k] = -leds[k];                     //invert
//    leds[i] += CRGB( 20, 0, 0);             //add a little red
//    leds[j] += CRGB( 0, 20, 0);             //add a little green
//    leds[k] += CRGB( 0, 0, 20);             //add a little blue
//
//    #ifdef DEBUG
//      Serial.print(MyDateAndTime.Hour);
//      Serial.print(":");
//      Serial.print(MyDateAndTime.Minute);
//      Serial.print(".");
//      Serial.print(MyDateAndTime.Second);
//      Serial.print(" - ");
//      Serial.print(MyDateAndTime.Minute);
//      Serial.print("/");
//      Serial.print(MyDateAndTime.Day);
//      Serial.print("/");
//      Serial.print(MyDateAndTime.Month);
//      Serial.print("/");
//      Serial.print(MyDateAndTime.Year);
//      Serial.println(" ");
//    #endif
//    
//    _previousMillis = currentMillis;        //save state for next time around
//  }
//}
//

