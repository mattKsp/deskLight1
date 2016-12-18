/*----------------------------utils----------------------------*/

void checkSegmentEndpoints() {
 fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
 
 leds[_segmentStart[0]] = CRGB(255, 0, 0);
 leds[_segmentEnd[0]] = CRGB(255, 0, 0);

 leds[_segmentStart[1]] = CRGB(0, 255, 0);
 leds[_segmentEnd[1]] = CRGB(0, 255, 0);
 
 leds[_segmentStart[2]] = CRGB(0, 0, 255);
 leds[_segmentEnd[2]] = CRGB(0, 0, 255);
 
 leds[_segmentStart[3]] = CRGB(0, 255, 0);
 leds[_segmentEnd[3]] = CRGB(0, 255, 0);
}


