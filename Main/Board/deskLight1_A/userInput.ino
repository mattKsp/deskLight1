/*----------------------------user input----------------------------*/

void setupUserInputs() {
  
//  //button0
//  pinMode(_button0Pin, INPUT_PULLUP);       //Setup the button with an internal pull-up :
//  _button0.attach(_button0Pin);             //After setting up the button, setup the Bounce instance :
//  _button0.interval(_buttonDebounceTime);
  
  //touch sensors already setup..
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  //button0();
  touchSensors();
}

///*----------------------------buttons----------------------------*/
//void button0() {
//  boolean hasChanged0 = _button0.update();  //Bounce buttons keep internal track of whether any change has occured since last time
//  
//  if(hasChanged0) {
//    //_button[0].update();                  //Update the LED button Bounce instance again  ???
//      //do button stuff
//      if(_button0.fell()) {
//        if(_onOff == false) {
//          _onOff = true;  //if the lights are already off, then turn them on
//          //don't need to change mode, as we are already in a mode, just switched on
//        } else {
//          //_modeCur +=1;
//          //if(_modeCur >= _modeNum){ _modeCur = 0; }  //TEMP rollover catch
//    
//          _modePresetSlotCur +=1;
//          if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
//          _modeCur = _modePreset[_modePresetSlotCur];
//          //write cur mode to memory ???
//        } //END onOff
//          #ifdef DEBUG
//          Serial.print(F("button 0 - fell"));
//          Serial.println();
//        #endif
//      } //END fell
//      if(_button0.rose()) {
//        //write cur mode to memory ???
//      } //END rose
//    
//  } //END hasChanged0
//  
//} //END button0

/*----------------------------touch sensors----------------------------*/
void touchSensors() {

  /*---------------touch sensor 0 - on/off--------------*/
  if(_touchToggled[0] == false) {
    long touch0read = _touch0.capacitiveSensor(_touchSensorRes);     //mode
    #ifdef DEBUG
      Serial.print(touch0read);
      //Serial.print(" 0toggled == false");
      Serial.println();
    #endif
    if(touch0read > _touchSensorThreshold) {
      _touchToggled[0] = true;                      //toggle so we can block re-bounce
      _touchPrevMillis[0] = millis();               //store the current time
      
      //_touchToggled[1] = true;  //try this aswell
      //_touchPrevMillis[1] = millis();               //store the current time
      
      _onOff = !_onOff;                         //flip the lights
         
      #ifdef DEBUG
        Serial.print(F("0touch 0 triggered"));
        Serial.println();
        Serial.print(F("on/off = "));
        Serial.print(_onOff);
        Serial.println();
      #endif
      
    } //END _touch0read > _touchSensorThreshold
  } //END _touchToggled[0] == false
    
  /*---------------touch sensor 1 - mode--------------*/
  if(_touchToggled[1] == false) {
    long touch1read = _touch1.capacitiveSensor(_touchSensorRes);     //mode
    #ifdef DEBUG
      Serial.print(touch1read);
      //Serial.print(" 1toggled == false");
      Serial.println();
    #endif
    if(touch1read > _touchSensorThreshold) {
      _touchToggled[1] = true;                    //toggle so we can block re-bounce
      //_touch0prevMicros = micros();               //store the current time
      _touchPrevMillis[1] = millis();               //store the current time
      
      //_touchToggled[0] = true;  //try this aswell
      //_touchPrevMillis[0] = millis();               //store the current time
      
      if(_onOff == false) {
        _onOff = true;  //if the lights are already off, then turn them on
        //don't need to change mode, as we are already in a mode, just switched on
      } else {
        incrementPresetSlot();
        //write cur mode to memory ???
      } //END onOff
      
      #ifdef DEBUG
        Serial.print(F("1touch 1 triggered"));
        Serial.println();
        Serial.print(F("Mode = "));
        Serial.print(_modeCur);
        Serial.println();
      #endif
    } //END _touch1read > _touchSensorThreshold
  } //END _touchToggled[1] == false

  for(int i = 0; i < 5; i++) {
    if(_touchToggled[i] == true) {
      long touchcurMillis = millis();            //get current micros() //unsigned long
      if((long) (touchcurMillis - _touchPrevMillis[i]) >= _touchDeBounceInterval) {  // if((unsigned long)..
        _touchToggled[i] = false;                 //reset
      }
    }
  }
  
} //END touchSensors


