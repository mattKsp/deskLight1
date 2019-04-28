/*----------------------------internal mesh messages----------------------------*/
/*---------------------get passed to/from MQTT broker by bridge-----------------*/

/*----------------------------messages - receive----------------------------*/
void receiveMessage(uint32_t from, String msg)
{
  uint8_t firstMsgIndex = msg.indexOf(':');
  String targetSub = msg.substring(0, firstMsgIndex);
  String msgSub = msg.substring(firstMsgIndex+1);

  if (targetSub == "lights/light/switch")
  {
    if (msgSub == LIGHTS_ON)
    {
      _onOff = true;
    }
    else if (msgSub == LIGHTS_OFF)
    {
      _onOff = false;
    }
    publishState(true);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  } 
  else if (targetSub == "lights/brightness/set")
  {
    uint8_t brightness = msgSub.toInt();
    if (brightness < 0 || brightness > 255) {
      // do nothing...
      return;
    } else {
      setGlobalBrightness(brightness);
      publishBrightness(true);
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(brightness); }
  }
  else if (targetSub == "lights/mode")
  {
    
    if (msgSub == "Glow") 
    { _modeCur = 0;
      _modePresetSlotCur = 0; }       // TEMP way of doing it.. until user preset slot changing is implemented.
    //else if (msgSub == "Sunrise") 
    //{ _modeCur = 1; }
    else if (msgSub == "Morning") 
    { _modeCur = 2;
      _modePresetSlotCur = 1; }
    else if (msgSub == "Day") 
    { _modeCur = 3;
      _modePresetSlotCur = 2; }
    else if (msgSub == "Working") 
    { _modeCur = 4;
      _modePresetSlotCur = 3; }
    else if (msgSub == "Evening") 
    { _modeCur = 5;
      _modePresetSlotCur = 4; }
    //else if (msgSub == "Sunset") 
    //{ _modeCur = 6; }
    else if (msgSub == "Night") 
    { _modeCur = 7;
      _modePresetSlotCur = 5; }
    else if (msgSub == "Effect") 
    { _modeCur = 8; }
    else { }

    _modeString = msgSub; // redundant ???
    
    //publishMode(true);  // too much bounce-back on the network
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/mode/coltemp")
  {
    if (msgSub == "Warm") 
    { setColorTemp(0); }
    else if (msgSub == "Standard") 
    { setColorTemp(1); }
    else if (msgSub == "CoolWhite") 
    { setColorTemp(2); }
    //publishColorTemp(true);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/mode/effect")
  {
    // 
    //publishEffect(true);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "sunrise")
  {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) {
      //start sunrise
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunrise and revert to previous setting
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/sunrise") {
    // trigger only
    // note: the single mesh msg of 'sunrise' is used for synced global sunrise
    if (msgSub == LIGHTS_ON) {
      //start sunrise
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunrise and revert to previous setting
      //publishMode(true);
    }
    //else if (msgSub == "receive a time for sunrise to happen at") {
    //set sunrise time
    //}
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "sunset")
  {
    // trigger only (global synced)
    if (msgSub == LIGHTS_ON) {
      //start sunset
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunset and revert to previous setting
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/sunset") {
    // trigger only
    // note: the single mesh msg of 'sunset' is used for synced global sunset
    if (msgSub == LIGHTS_ON) {
      //start sunset
    }
    else if (msgSub == LIGHTS_OFF) {
      //stop sunset and revert to previous setting
    }
    //else if (msgSub == "receive a time for sunset to happen at") {
    //set sunset time
    //}
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  /*
   * Breath : (noun) Refers to a full cycle of breathing. It can also refer to the air that is inhaled or exhaled.
   */
  else if (targetSub == "lights/breath")
  {
    if (msgSub == LIGHTS_ON) {
      
      //publishMode(true);
    }
    else if (msgSub == LIGHTS_OFF) {
      
      //publishMode(true);
    }
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/breath/xyz")
  {
    // msg will contain xyz coords for origin position within the house
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "lights/breath/xyz/mode")
  {
    // set positional mode
    // independent, global
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if(targetSub == "debug/general/set") 
  {
    if(msg == LIGHTS_ON) { DEBUG_GEN = true; } 
    else if(msg == LIGHTS_OFF) { DEBUG_GEN = false; }
    publishDebugGeneralState(false);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "debug/overlay/set")
  {
    if (msgSub == LIGHTS_ON) { DEBUG_OVERLAY = true; }
    else if (msgSub == LIGHTS_OFF) { DEBUG_OVERLAY = false; }
    publishDebugOverlayState(false);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if (targetSub == "debug/meshsync/set")
  {
    if (msgSub == LIGHTS_ON) { DEBUG_MESHSYNC = true; }
    else if (msgSub == LIGHTS_OFF) { DEBUG_MESHSYNC = false; }
    publishDebugMeshsyncState(false);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  else if(targetSub == "debug/comms/set") 
  {
    if(msg == LIGHTS_ON) { DEBUG_COMMS = true; } 
    else if(msg == LIGHTS_OFF) { DEBUG_COMMS = false; }
    publishDebugCommsState(false);
    //if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
  }
  
  if (DEBUG_COMMS) { Serial.print(targetSub); Serial.print(" : "); Serial.println(msgSub); }
}


/*----------------------------messages - publish----------------------------*/
void publishState(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishState "); }
  String msg = "lights/light/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if(_onOff == false) {
    msg += "OFF";
  } else {
    msg += "ON";
  }
  mesh.sendSingle(id, msg);
  //mesh.sendBroadcast(msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishBrightness(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishBrightness "); }
  String msg = "lights/brightness/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  msg += String(_ledGlobalBrightnessCur);
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishMode(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishMode "); }
  String msg = "lights/mode";
  msg += ":"; //..just so we are all sure what is going on here !?
  msg += modeName[_modeCur];
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishSubMode(bool save) { /* TODO - but might just use ColTemp */ }

void publishColorTemp(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishColorTemp "); }
  String msg = "lights/mode/coltemp";   // NOTE this needs to be 'colTemp'
  msg += ":"; //..just so we are all sure what is going on here !?
  msg += colorTempName[_colorTempCur];
  //mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishEffect(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishEffect "); }
  String msg = "lights/mode/effect";
  msg += ":"; //..just so we are all sure what is going on here !?
  //msg += ;
  //mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishDebugGeneralState(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishDebugGeneralState "); }
  String msg = "debug/general/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if (DEBUG_GEN == false) {
    msg += "OFF";
  } else if (DEBUG_GEN == true) {
    msg += "ON";
  }
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishDebugOverlayState(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishDebugOverlayState "); }
  String msg = "debug/overlay/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if (DEBUG_OVERLAY == false) {
    msg += "OFF";
  } else if (DEBUG_OVERLAY == true) {
    msg += "ON";
  }
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishDebugMeshsyncState(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishDebugMeshsyncState "); }
  String msg = "debug/meshsync/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if (DEBUG_MESHSYNC == false) {
    msg += "OFF";
  } else if (DEBUG_MESHSYNC == true) {
    msg += "ON";
  }
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}

void publishDebugCommsState(bool save)
{
  if (DEBUG_COMMS) { Serial.print("publishDebugCommsMeshsyncState "); }
  String msg = "debug/comms/status";
  msg += ":"; //..just so we are all sure what is going on here !?
  if (DEBUG_COMMS == false) {
    msg += "OFF";
  } else if (DEBUG_COMMS == true) {
    msg += "ON";
  }
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}
