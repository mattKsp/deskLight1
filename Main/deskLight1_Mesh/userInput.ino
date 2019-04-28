/*----------------------------user input----------------------------*/
boolean _touchActive = false;
void setupUserInputs() {
  s32 ret = 0;                                  // s32 = int
  if(mpr121.begin() < 0)
  {
    _touchActive = false;
    if (DEBUG_USERINPUT) { Serial.println("Can't detect MPR121 device!!!!"); }
  }
  else
  {
    _touchActive = true;
    if (DEBUG_USERINPUT) { Serial.println("MPR121 device init OK!"); }
  }

  delay(100);
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  if (_touchActive) {
    touchSensorsMPR121();
  }
}

/*---------------touch sensors MPR121--------------*/
void touchSensorsMPR121() {
  u16 result = 0;                               // u16 = unsigned short
  u16 filtered_data_buf[CHANNEL_NUM] = { 0 };
  u8 baseline_buf[CHANNEL_NUM] = { 0 };         // u8 = unsigned char
  
  result = mpr121.check_status_register();

  mpr121.get_filtered_reg_data(&result, filtered_data_buf);

  for(int i = 0; i < CHANNEL_NUM; i++)
  {
    if (result&(1<<i))                             /*key i is pressed!!*/
    {
      if(0 == touch_status_flag[i])             
      { 
        touch_status_flag[i] = 1;
        
        if (i == 0) {
              touch0pressed();
        } else { 
          if (_onOff == false) {
            _onOff = true;  //if the lights are already off, then turn them on
            //don't need to do anything else as we are already in a mode etc.
            publishState(true);
          } else {
             if (i == 1) {
              touch1pressed();
            } else if (i == 2) {
              touch2pressed();
            } else if (i == 3) {
              touch3pressed();
            } else if (i == 4) {
              touch4pressed();
            }
          } // end if else _onOff = false
        } // end if i == 0
        
        if (DEBUG_USERINPUT) { 
          Serial.print("Touch ");
          Serial.print(i);
          Serial.println(" pressed.");
        }
      } // end if touch status flag 0
    } // end if result
    else
    {
      if(1 == touch_status_flag[i])
      {
        touch_status_flag[i] = 0;

        if (DEBUG_USERINPUT) { 
          Serial.print("Touch ");
          Serial.print(i);
          Serial.println(" released.");
        }
      } // end  if touch status flag 1
    } // end else result
  } // end for-loop
  //delay(50);                                  // ???
}

/*---------------touch sensors MPR121 - pressed--------------*/
void touch0pressed() {                          // touch sensor 0 - on/off
  _onOff = !_onOff;                             // flip the lights
  publishState(true);
}
void touch1pressed() {                          // touch sensor 1 - mode up
  incrementPresetSlot();
  publishMode(true);
}
void touch2pressed() {                          // touch sensor 2 - mode down
  decrementPresetSlot();
  publishMode(true);
}
void touch3pressed() {                          // touch sensor 3 - sub-mode cycle
  // sub-modes eg. cycle temperature modes (not implemented yet)
  cycleColorTemp();
  publishColorTemp(true);
}
void touch4pressed() {                          // touch sensor 4 - brightness up
  //increaseBrightness();
  //publishBrightness(true);
}
void touch5pressed() {                          // touch sensor 5 - brightness down
  //decreaseBrightness();
  //publishBrightness(true);
}

/*---------------touch sensors MPR121 - released--------------*/
