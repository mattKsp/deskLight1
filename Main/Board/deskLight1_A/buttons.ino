/*----------------------------buttons----------------------------*/

void setupButtons() {
  
    //button0
    pinMode(BUTTON_0_PIN, INPUT_PULLUP);          //Setup the button with an internal pull-up :
    
    _button0.attach(BUTTON_0_PIN);                //After setting up the button, setup the Bounce instance :
    _button0.interval(_buttonDebounceTime);

    //
}

void button0() {
  boolean hasChanged0 = _button0.update();      //Bounce buttons keep internal track of whether any change has occured since last time
  
  if (hasChanged0) {
    //_button[0].update();                        //Update the LED button Bounce instance again  ???
    
    if(_button0.fell()) {
      _mode0_sub += 1;
      if(_mode0_sub >= 9){ _mode0_sub = 0; }  //TEMP rollover
        
      //..messy
      if(_mode == 0) {
        _mode0_sub += 1;
        if(_mode0_sub >= 9){ _mode0_sub = 0; }  //TEMP rollover
      } 
      else if(_mode == 1) {
        _button0Toggled = !_button0Toggled;
        _ledState = !_ledState;
        digitalWrite(LED_PIN, _ledState );
      } 
      else if(_mode == 2) {
        _mode2_sub += 1;
        if(_mode2_sub >= 4){ _mode2_sub = 0; }  //TEMP rollover
      }

    }
    if(_button0.rose()) { }
    
  } //END hasChanged0
  
} //END button0


//for (i = 0; i < (sizeof(myInts)/sizeof(int)) - 1; i++) {
//  // do something with myInts[i]
//}

