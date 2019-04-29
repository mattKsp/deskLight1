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

void publishStatusAll(bool save) {
  
  if (DEBUG_COMMS) { Serial.println("publishStatusAll "); }
  publishState(save);
  publishBrightness(save);
  publishMode(save);
  publishColorTemp(save);
  publishDebugGeneralState(save);
  publishDebugOverlayState(save);
  publishDebugMeshsyncState(save);
  publishDebugCommsState(save);
}
