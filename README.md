# deskLight1
LED desk light, by Thurstan

Repository is an Arduino project

### Equipment
#### Hardware
- ~~Arduino Mini Pro board - 5v, ATmega328, 16 MHz, 1K EEPROM~~
- WeMos D1 (R2 &) mini
  * 80 MHz 
  * 115200 baud 
  * 4M (3M SPIFFS)
- ~~DS3231 RTC (realtime clock) with AT24C32 memory backpack - 5v, I2C~~
- WS2812B addressable LED strips
  * pixel order = GRB
- 12 Key Capacitive I2C Touch Sensor V2 (MPR121)
  * 3.3v
  * I2C
  * The SDA, SCL and the interrupt pin jumpers connect to 10k pull-up resistors. If pull-up resistors are not needed cut the traces.

#### Software
- Arduino 
- ~~EEPROM libary~~
- Bounce library
- CapacitiveSensor library  https://github.com/PaulStoffregen/CapacitiveSensor
- ~~DS3231_Simple library  https://github.com/sleemanj/DS3231_Simple~~
  * ~~DS3231 I2C address = 0x68~~
  * ~~AT24C32 I2C address = 0x57~~
- FastLED library  https://github.com/FastLED/FastLED
- Touch sensor library  https://github.com/linux-downey/Grove_touch_sensor_MPR121
  * I2C address = 0x5A (90)

### Licence
- Written by MTS Standish (Thurstan|mattKsp) 2016
- Released under GNU GPLv3 (see LICENCE file)

This software is provided on an "AS-IS-BASIS"

### Reference
https://github.com/FastLED/FastLED/wiki/Pixel-reference <br> http://playground.arduino.cc/Main/CapacitiveSensor <br> http://www.timeanddate.com/sun/uk/york <br>  
 
