/*----------------------------user input----------------------------*/

void setupUserInputs() {
  
  //button0
  pinMode(_button0Pin, INPUT_PULLUP);       //Setup the button with an internal pull-up :
  _button0.attach(_button0Pin);             //After setting up the button, setup the Bounce instance :
  _button0.interval(_buttonDebounceTime);
  
  //setupTouchSensors();
  
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  button0();
  //touchSensors();
}

/*----------------------------buttons----------------------------*/
void button0() {
  boolean hasChanged0 = _button0.update();  //Bounce buttons keep internal track of whether any change has occured since last time
  
  if(hasChanged0) {
    //_button[0].update();                  //Update the LED button Bounce instance again  ???
      //do button stuff
      if(_button0.fell()) {
        if(_onOff == false) {
          _onOff = true;  //if the lights are already off, then turn them on
          //don't need to change mode, as we are already in a mode, just switched on
        } else {
          //_modeCur +=1;
          //if(_modeCur >= _modeNum){ _modeCur = 0; }  //TEMP rollover catch
    
          _modePresetSlotCur +=1;
          if(_modePresetSlotCur >= _modePresetSlotNum){ _modePresetSlotCur = 0; }  //TEMP rollover catch
          _modeCur = _modePreset[_modePresetSlotCur];
          //write cur mode to memory ???
          DEBUG_PRINT("user input - button 0 fell")
        } //END onOff
      } //END fell
      if(_button0.rose()) {
        //write cur mode to memory ???
        DEBUG_PRINT("user input - button 0 rose")
      } //END rose
    
  } //END hasChanged0
  
} //END button0

/*----------------------------touch sensors----------------------------*/
void touchSensors() {

  //for(int i = 0; i < _touchSensorsTotal; i++) {
    
    //if touch && nothing turned on
    //- turn on to default state
  
    //if touch && already something turned on
    //- do switching stuff
    
  //}
  
} //END touchSensors


//for (i = 0; i < (sizeof(myInts)/sizeof(int)) - 1; i++) {
//  // do something with myInts[i]
//}

