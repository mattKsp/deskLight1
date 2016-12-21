/*----------------------------util----------------------------*/

/*
  DS3231 - kick the interrupt
  TEMP - just to make sure interrupt output pin SQW gets turned on
  enables interrupt pin, alarm 2 and alarm
 */
void DS3231kickInterrupt() {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);                         //control location
  Wire.write(B00000111);                    //..this!
  Wire.endTransmission();
}

/*
 Check Segment Endpoints
 usage | checkSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void checkSegmentEndpoints() {
  fill_solid( leds, _ledNum, CRGB(0,0,0));
  
  leds[ledSegment[0].first] = CRGB(255, 0, 0);
  leds[ledSegment[0].last] = CRGB(255, 0, 0);
  
  leds[ledSegment[1].first] = CRGB(0, 255, 0);
  leds[ledSegment[1].last] = CRGB(0, 255, 0);
  
  leds[ledSegment[2].first] = CRGB(0, 0, 255);
  leds[ledSegment[2].last] = CRGB(0, 0, 255);
  
  leds[ledSegment[3].first] = CRGB(0, 255, 0);
  leds[ledSegment[3].last] = CRGB(0, 255, 0);
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  leds[0] = _colorTempCur;                  //show indicator pixel
  //leds[0] = TEMPERATURE_1;                  //show indicator pixel
}

/*
 Get Voltage
 usage | Serial.println( getVoltage(), DEC );
 This compares Vcc against internal 1.1v reference and returns the result in mV (eg. 5000mV = 5v)
 ATmega168 & 328 chips can use this trick to read their own voltage rails
 https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
 */
long getVoltage() { 
  long result;                              // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2);                                 // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC);                      // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = 1126400L / result;               // Back-calculate AVcc in mV
  return result;
}

/*
 * Serial promt to set alarm 1
 */
#ifdef SET_ALARM1_BY_SERIAL
void promptForAlarm1(Stream &Serial)
{
  char buffer[3] = { 0 };
    
  Serial.println("Alarm 1 is set when all data is entered and you send 'Y' to confirm.");
  do
  {
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Hour (2 digits, 24 hour clock, 00-23): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int hr = atoi(buffer[0] == '0' ? buffer+1 : buffer);
    
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Minute (2 digits, 00-59): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int mins = atoi(buffer[0] == '0' ? buffer+1 : buffer);

    Serial.println();
    Serial.print("Entered Alarm 1 - "); 
    //printTo(Serial, Settings);
    Serial.print(hr);
    Serial.print(":"); 
    Serial.print(mins); 
     
    Serial.println();
    Serial.print("Send 'Y' to set the Alarm 1, send 'N' to start again: ");
        
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 1);
    while(Serial.available()) Serial.read(); 
    if(buffer[0] == 'Y' || buffer[0] == 'y')
    {
      setSunRise(1, hr, mins);    //mode 1 = set time this way
      Serial.println();
      Serial.print("Alarm 1 set"); 
      break;
    }
  } while(1);   
}
#endif

/*
 * Serial promt to set alarm 2
 */
#ifdef SET_ALARM2_BY_SERIAL
void promptForAlarm2(Stream &Serial)
{
  char buffer[3] = { 0 };
    
  Serial.println("Alarm 2 is set when all data is entered and you send 'Y' to confirm.");
  do
  {
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Hour (2 digits, 24 hour clock, 00-23): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int hr = atoi(buffer[0] == '0' ? buffer+1 : buffer);
    
    memset(buffer, 0, sizeof(buffer));
    Serial.println();
    Serial.print("Enter Minute (2 digits, 00-59): ");
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 2);
    while(Serial.available()) Serial.read(); 
    int mins = atoi(buffer[0] == '0' ? buffer+1 : buffer);

    Serial.println();
    Serial.print("Entered Alarm 2 - "); 
    Serial.print(hr);
    Serial.print(":"); 
    Serial.print(mins); 
     
    Serial.println();
    Serial.print("Send 'Y' to set the Alarm 2, send 'N' to start again: ");
        
    while(!Serial.available()) ; // Wait until bytes
    Serial.readBytes(buffer, 1);
    while(Serial.available()) Serial.read(); 
    if(buffer[0] == 'Y' || buffer[0] == 'y')
    {
      setSunSet(0, hr, mins);    //mode 1 = set time this way
      Serial.println();
      Serial.print("Alarm 2 set"); 
      break;
    }
  } while(1);   
}
#endif

