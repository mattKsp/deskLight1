/*----------------------------mode1 sunrise and mode6 sunset----------------------------*/
/*
 * sunrise and sunset modes (1 & 6) are usually kept out of the preset lists and just triggered by the alarm interrupts
 * ..but it is a mode and can be selected as such 
 * eg. for testing/demo porpoises
 */
void mode1() {
  
  //name should be sunrise!
  //isStatic default is false
  //if triggered by an alarm, then temporarily make sure isStatic is set to false
  if(modeInfo[1].isStatic) {
    //
  }
  else {
    //
  }
  
} //END mode1


void mode6() {
  
  //name should be sunset!
  //isStatic default is false
  if(modeInfo[6].isStatic) {
    //
  }
  else {
    //
  }
  
} //END mode6



