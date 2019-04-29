/*----------------------------display----------------------------*/

void setupLEDs() {
  
  strip.Begin();                                  // NeoPixelBus
  strip.Show();                                   // NeoPixelBus

  //uint8_t oldBrightness = strip.GetBrightness();
  strip.SetBrightness(_ledGlobalBrightnessCur);
}

/*
 Fade all leds except for the sacrificial one in the box
 LED '0' is used for debug porpoises
 */
void FadeAll(uint8_t darkenBy) {
    RgbColor color;
    for (uint16_t i = 1; i < strip.PixelCount(); i++)
    {
        color = strip.GetPixelColor(i);
        if (color.R > 0 && color.G > 0 && color.B > 0) {
          color.Darken(darkenBy);
        } else {
          color = _rgbBlack;
        }
        strip.SetPixelColor(i, color);
    }
}

/*
 A gradient of colour from one end of a strip to the other
 */
void FillGradientRGB(byte first, byte total, RgbColor colA, RgbColor colB) {
  for (uint16_t indexPixel = 0; indexPixel < total; indexPixel++)
    {
      //map number of used pixels to 0-100
      //mapf();
      int ti = map(indexPixel, 0, (total+1), 0, 100);
      
      //divide by 100 for 0.0-1.0
      float tf = ti/100.0;
      
      RgbColor updatedColor = RgbColor::LinearBlend(colA, colB, tf);
      
      strip.SetPixelColor( (indexPixel + first), updatedColor);
      
      if (DEBUG_GEN) { 
        Serial.print("SetGradient - i "); 
        Serial.print(indexPixel); 
        Serial.print(", ti - "); 
        Serial.print(ti); 
        Serial.print(", tf - "); 
        Serial.print(tf); 
        Serial.print(", col - "); 
        Serial.print(updatedColor.R); 
        Serial.print(", "); 
        Serial.print(updatedColor.G); 
        Serial.print(", "); 
        Serial.print(updatedColor.B); 
        Serial.println(); 
      }
    }
}

/*
 Check Segment Endpoints
 usage | showSegmentEndpoints();
 overlay/place near end of chain
 This places different coloured lights at the ends of the LED strip(s) segments for quick visual feedback of calculations
 */
void showSegmentEndpoints() {
  strip.ClearTo(_rgbBlack);
  
  strip.SetPixelColor(ledSegment[1].first, _rgbRed);
  strip.SetPixelColor(ledSegment[1].last, _rgbRed);
  
  strip.SetPixelColor(ledSegment[2].first, _rgbGreen);
  strip.SetPixelColor(ledSegment[2].last, _rgbGreen);
  
  strip.SetPixelColor(ledSegment[3].first, _rgbRed);
  strip.SetPixelColor(ledSegment[3].last, _rgbRed);
  
  strip.SetPixelColor(ledSegment[4].first, _rgbBlue);
  strip.SetPixelColor(ledSegment[4].last, _rgbBlue);
}

/*
 Show colour temperature indicator pixel
 usage | showColorTempPx();
 overlay/place near end of chain
 This sets the first LED to show the current colour temperature
 */
void showColorTempPx() {
  strip.SetPixelColor(ledSegment[4].last, _rgbColorTempCur);
}
