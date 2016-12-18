/*----------------------------buttons----------------------------*/

void setupButtons() {
    //button0
    pinMode(_button0Pin, INPUT_PULLUP); //Setup the button with an internal pull-up :
    //After setting up the button, setup the Bounce instance :
    _button0.attach(_button0Pin);
    _button0.interval(_buttonDebounceTime);
}

void button0() {
  if(_buttonsEnabled) {

    boolean hasChanged0 = _button0.update();  //Bounce buttons keep internal track of whether any change has occured since last time
    
    if (hasChanged0) {
      //_button[0].update(); //Update the LED button Bounce instance  ???
      
      if(_button0.fell()) {

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
      
    } //END hasChanged0
  
  }
} //END button0


//for (i = 0; i < (sizeof(myInts)/sizeof(int)) - 1; i++) {
//  // do something with myInts[i]
//}


    //int btTotal = 1;
    //for(int i = 0; i < btTotal; i++) {
      
    // Update the Bounce instance :
    //debouncer.update();
  
//    if (_button[i].fallingEdge()) {
//      boolean btTEMP = false;
//      if(_buttonToggled[i] == false) {
//        btTEMP = true;
//      } else if(_buttonToggled[i] == true) {
//        btTEMP = false;
//      }
//      _buttonToggled[i] = btTEMP;
//    }
//  
//    if (_button[i].risingEdge()) {
//      
//    }

  //boolean ledPinTEMP = false;
  
//    boolean hasChanged0 = _button[0].update();  //Bounce buttons keep internal track of whether any change has occured since last time
//    if (hasChanged0) {
//      //_button[0].update(); //Update the LED button Bounce instance
//      if(_button[0].fell()) {
//        _buttonToggled[0] = !_buttonToggled[0];
//        _ledState = !_ledState;
//        digitalWrite(LED_PIN, _ledState );
//      }
//    }

  //ledButton();
  
  //digitalWrite(LED_PIN, HIGH );

  //} //end for < btTotal

  /*
  // Get the updated value :
  int value = debouncer.read();

  // Turn on or off the LED as determined by the state :
  if ( value == LOW ) {
    digitalWrite(LED_PIN, HIGH );
    //buttonToggled[0] != buttonToggled[0];
    boolean btTEMP = false;
    if(_buttonToggled[0] == false) {
      btTEMP = true;
    } else if(_buttonToggled[0] == true) {
      btTEMP = false;
    }
    _buttonToggled[0] = btTEMP;
  } else if ( value == HIGH ) {
    digitalWrite(LED_PIN, LOW );
    //buttonToggled[0] = false;
  }
  */



