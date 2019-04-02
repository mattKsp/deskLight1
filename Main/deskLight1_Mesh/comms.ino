/*----------------------------comms----------------------------*/
//void comms() { }

void setupMesh() 
{
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  //mesh.init( MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, STATION_CHANNEL );
  //mesh.init( MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_STA, STATION_CHANNEL );
  mesh.init(MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, STATION_CHANNEL, 1, 4); // hidden
  
  //mesh.init(MESH_NAME, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, STATION_CHANNEL, uint8_t hidden = 0, uint8_t maxconn = MAX_CONN);
  
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

//  blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
//      // If on, switch off, else switch on
//      if (onFlag)
//        onFlag = false;
//      else
//        onFlag = true;
//      blinkNoNodes.delay(BLINK_DURATION);
//
//      if (blinkNoNodes.isLastIteration()) {
//        // Finished blinking. Reset task for next run 
//        // blink number of nodes (including this node) times
//        blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
//        // Calculate delay based on current mesh time and BLINK_PERIOD
//        // This results in blinks between nodes being synced
//        blinkNoNodes.enableDelayed(BLINK_PERIOD - 
//            (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
//      }
//  });
//  userScheduler.addTask(blinkNoNodes);
//  blinkNoNodes.enable();
}
