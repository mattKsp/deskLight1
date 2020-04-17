/*----------------------------messages - publish----------------------------*/
void meshSendSingle(String nom, String msg, bool save) {
  if (DEBUG_COMMS) { Serial.print(nom); Serial.print(" - "); }
  mesh.sendSingle(id, msg);
  if (DEBUG_COMMS) { Serial.println(msg); }
  if (save == true) { shouldSaveSettings = true; }
}
void publishMeshMsgSingleState(String nom, String addr, boolean state, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  if (state == false) {  addr += "OFF"; }
  else if (state == true) { addr += "ON"; }
  meshSendSingle(nom, addr, save);
}
void publishMeshMsgSingleString(String nom, String addr, String msg, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  addr += msg;
  meshSendSingle(nom, addr, save);
}
void publishMeshMsgSingleColor(String nom, String addr, uint8_t r, uint8_t g, uint8_t b, bool save) {
  addr += ":"; //..just so we are all sure what is going on here !?
  addr += String(r);
  addr += ",";
  addr += String(g);
  addr += ",";
  addr += String(b);
  meshSendSingle(nom, addr, save);
}

/*----------------------------messages - publish - main---------------------*/
void publishState(bool save) {
  publishMeshMsgSingleState("publishState", "lights/light/status", _onOff, save);
}

void publishBrightness(bool save) {
  publishMeshMsgSingleString("publishBrightness", "lights/brightness/status", String(_ledGlobalBrightnessCur), save);
}

void publishMode(bool save) {
  publishMeshMsgSingleString("publishMode", "lights/mode", modeName[_modeCur], save);
}

void publishColorTemp(bool save) {
  publishMeshMsgSingleString("publishColorTemp", "lights/mode/coltemp", colorTempName[_colorTempCur], save);
}

void publishEffect(bool save) { }

void publishDebugGeneralState(bool save) {
  publishMeshMsgSingleState("publishDebugGeneralState", "debug/general/status", DEBUG_GEN, save);
}

void publishDebugOverlayState(bool save) {
  publishMeshMsgSingleState("publishDebugOverlayState", "debug/overlay/status", DEBUG_OVERLAY, save);
}

void publishDebugMeshsyncState(bool save) {
  publishMeshMsgSingleState("publishDebugMeshsyncState", "debug/meshsync/status", DEBUG_MESHSYNC, save);
}

void publishDebugCommsState(bool save) {
  publishMeshMsgSingleState("publishDebugCommsState", "debug/comms/status", DEBUG_COMMS, save);
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
