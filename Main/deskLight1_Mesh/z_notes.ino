/*----------------------------notes----------------------------*/

/* 
 * WeMos D1 (R2 &) mini, 80 MHz, 115200 baud, 4M, (1M SPIFFS)
 * 
 * Pinout Wemos D1 Mini (ESP-8266)
 * RX  3
 * TX  1
 * A0  Analog input, max 3.2V input  A0
 * D0  16  IO  GPIO16   - no PWM or I2C or interrupt
 * D1  5  IO, SCL GPIO5  I2C
 * D2  4  IO, SDA GPIO4  I2C
 * D3  0  IO, 10k Pull-up GPIO0
 * D4  2  IO, 10k Pull-up, BUILTIN_LED  GPIO2
 * D5  14  IO, SCK GPIO14
 * D6  12  IO, MISO  GPIO12
 * D7  13  IO, MOSI  GPIO13
 * D8  15  IO, 10k Pull-down, SS GPIO15
 * 
 * 
 * Grove - 12 Key Capacitive I2C Touch Sensor V2 (MPR121)
 * http://wiki.seeedstudio.com/Grove-12_Key_Capacitive_I2C_Touch_Sensor_V2-MPR121/
 * 
 * 
 * https://hackaday.com/2017/01/20/cheating-at-5v-ws2812-control-to-use-a-3-3v-data-line/
 * https://github.com/FastLED/FastLED/wiki/Interrupt-problems
 * 
 */


/* 
 * from FastLED color.h
 *  
 * TypicalSMD5050=0xFFB0F0 - 255, 176, 240
 * 
 * 
 *
 * @name Black-body radiation light sources
 * Black-body radiation light sources emit a (relatively) continuous
 * spectrum, and can be described as having a Kelvin 'temperature'
 * 
 * 1900 Kelvin
 * Candle = 0xFF9329 - 255, 147, 41
 * 2600 Kelvin
 * Tungsten40W = 0xFFC58F - 255, 197, 143
 * 2850 Kelvin
 * Tungsten100W = 0xFFD6AA - 255, 214, 170
 * 3200 Kelvin
 * Halogen = 0xFFF1E0 - 255, 241, 224
 * 5200 Kelvin
 * CarbonArc = 0xFFFAF4 - 255, 250, 244
 * 5400 Kelvin
 * HighNoonSun = 0xFFFFFB - 255, 255, 251
 * 6000 Kelvin
 * DirectSunlight = 0xFFFFFF - 255, 255, 255
 * 7000 Kelvin
 * OvercastSky = 0xC9E2FF - 201, 226, 255
 * 20000 Kelvin
 * ClearBlueSky = 0x409CFF - 64, 156, 255
 * 
 * 
 * @name Gaseous light sources
 * Gaseous light sources emit discrete spectral bands, and while we can
 * approximate their aggregate hue with RGB values, they don't actually
 * have a proper Kelvin temperature.
 *
 * WarmFluorescent = 0xFFF4E5 - 255, 244, 229
 * StandardFluorescent = 0xF4FFFA - 244, 255, 250
 * CoolWhiteFluorescent = 0xD4EBFF - 212, 235, 255
 * FullSpectrumFluorescent = 0xFFF4F2 - 255, 244, 242
 * GrowLightFluorescent = 0xFFEFF7 - 255, 239, 247
 * BlackLightFluorescent = 0xA700FF - 167, 0, 255
 * MercuryVapor = 0xD8F7FF - 216, 247, 255
 * SodiumVapor = 0xFFD1B2 - 255, 209, 178
 * MetalHalide = 0xF2FCFF - 242, 252, 255
 * HighPressureSodium = 0xFFB74C - 255, 183, 76
 * 
 */
