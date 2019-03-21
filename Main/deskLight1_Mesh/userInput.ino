/*----------------------------user input----------------------------*/

void setupUserInputs() {
  s32 ret = 0;                                  // s32 = int
  if(mpr121.begin() < 0)
  {
    if (DEBUG) { Serial.println("Can't detect device!!!!"); }
  }
  else
  {
    if (DEBUG) { Serial.println("mpr121 init OK!"); }
  }
  delay(100);
}

/*
 * user inputs loop
 * called from main loop
 */
void loopUserInputs() {
  touchSensorsMPR121();
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
    if(result&(1<<i))                             /*key i is pressed!!*/
    {
      if(0 == touch_status_flag[i])             
      { 
        touch_status_flag[i] = 1;

        if (i == 0) {
          touch0pressed();
        } else if (i == 1) {
          touch1pressed();
        } else if (i == 2) {
          touch2pressed();
        } else if (i == 3) {
          touch3pressed();
        } else if (i == 4) {
          touch4pressed();
        }
        
        if (DEBUG) { 
          Serial.print("key ");
          Serial.print(i);
          Serial.println("pressed");
        }
      }
    }
    else
    {
      if(1 == touch_status_flag[i])
      {
        touch_status_flag[i] = 0;

        if (DEBUG) { 
          Serial.print("key ");
          Serial.print(i);
          Serial.println("release");
        }
      }
    }
  }
  //delay(50);                                  // ???
}

/*---------------touch sensors MPR121 - pressed--------------*/
void touch0pressed() {                          // touch sensor 0 - on/off
  _onOff = !_onOff;                             // flip the lights
}
void touch1pressed() {                          // touch sensor 1 - mode up
  if(_onOff == false) {
    _onOff = true;  //if the lights are already off, then turn them on
    //don't need to change mode, as we are already in a mode, just switched on
  } else {
    incrementPresetSlot();
  }
}
void touch2pressed() {                          // touch sensor 2 - mode down
  if(_onOff == false) {
    _onOff = true;  //if the lights are already off, then turn them on
    //don't need to change mode, as we are already in a mode, just switched on
  } else {
    decrementPresetSlot();
  }
}
void touch3pressed() {                          // touch sensor 3 - sub-mode cycle
  // sub-modes eg. cycle temperature modes (not implemented yet)
}
void touch4pressed() {                          // touch sensor 4 - brightness up
  //increaseBrightness();
}
void touch5pressed() {                          // touch sensor 5 - brightness down
  //decreaseBrightness();
}

/*---------------touch sensors MPR121 - released--------------*/


