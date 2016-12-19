/*----------------------------user input----------------------------*/

void setupUserInputs() {
  
  //button0
    pinMode(BUTTON_0_PIN, INPUT_PULLUP);          //Setup the button with an internal pull-up :
    _button0.attach(BUTTON_0_PIN);                //After setting up the button, setup the Bounce instance :
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
  boolean hasChanged0 = _button0.update();      //Bounce buttons keep internal track of whether any change has occured since last time
  
  if (hasChanged0) {
    //_button[0].update();                        //Update the LED button Bounce instance again  ???
    
    if(_button0.fell()) {
      //_modeCur +=1;
      //if(_modeCur >= MODE_NUM){ _modeCur = 0; }  //TEMP rollover

      _modePresetSlotCur +=1;
      if(_modePresetSlotCur >= MODE_PRESET_SLOT_NUM){ _modePresetSlotCur = 0; }  //TEMP rollover
      _modeCur = _modePreset[_modePresetSlotCur];
      //write cur mode to memory ???
      DEBUG_PRINT("user input - button 0 fell")
    }
    if(_button0.rose()) {
      //write cur mode to memory ???
      DEBUG_PRINT("user input - button 0 rose")
    }
    
  } //END hasChanged0
  
} //END button0

/*----------------------------touch sensors----------------------------*/
//int _touchSensorsTotal = 3;
boolean _touchSensorToggled[3];

void setupTouchSensors() {
  //
}

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

