/* Library to interface with RGB LEDs using the HSI color space.

   Author: http://github.com/nurblieh - Bradley Heilbrun
*/

#ifndef HSILight_H
#define HSILight_H

#include <Arduino.h>

//#define HSI_DEBUG 1

#define DEBUG_PRINTER Serial

#ifdef HSI_DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif  // HSI_DEBUG

class HsiLight {
 public:
  HsiLight(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
  void setHsi(float h, float s, float i);
  void fade(float h, float s, float i, uint16_t duration);
  void candleFlicker(uint16_t duration);
  float getHue();
  float getSaturation();
  float getIntensity();
  static void hsi2Rgb(float H, float S, float I, uint8_t rgb[]);
 private:
  void _setRgb(uint8_t rgb[]);
  uint8_t _rgbPins[3];
  // TODO: If float math is a perf issue, convert to longs.
  float _hsi[3];  // current state
};

#endif // HSILight_H
