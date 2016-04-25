/* Library to interface with RGB LEDs using the HSI color space.

   Author: http://github.com/nurblieh - Bradley Heilbrun
*/

#include <HSILight.h>

/* 
   HsiLight constructor.
   Args:
     redPin/greenPin/bluePin: PWM pins connected to RGB LED.
*/
HsiLight::HsiLight(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
  _rgbPins[0] = redPin;
  _rgbPins[1] = greenPin;
  _rgbPins[2] = bluePin;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  _hsi[0] = 0; _hsi[1] = 1; _hsi[2] = 0;
}

/* Set the state of our LED (immediately) to the specified h/s/i. */
void HsiLight::setHsi(float h, float s, float i) {
  uint8_t rgb[3];
  _hsi[0] = h; _hsi[1] = s; _hsi[2] = i;

  DEBUG_PRINT("Setting HSI: ");
  DEBUG_PRINT(h); DEBUG_PRINT(", ");
  DEBUG_PRINT(s); DEBUG_PRINT(", ");
  DEBUG_PRINT(i); DEBUG_PRINT("\n");

  hsi2Rgb(h, s, i, rgb); // results stored in rgb.
  _setRgb(rgb);
}

/* 
   Flicker LED like a candle.
   Args:
     duration: How long to flicker before returning.
*/
void HsiLight::candleFlicker(uint16_t duration) {
  uint16_t t, d;
  float hsiNew[3];
  // HSI range to emulate a candle's light.
  float hsiMin[3] = { 5.0, 0.8, 0.2};
  float hsiMax[3] = {12.0, 1.0, 1.0};

  t = 0;
  while (t < duration) {
    for (int i = 0; i < 3; i++)
      hsiNew[i] = float(random(hsiMin[i]*100, hsiMax[i]*100)) / 100.0;
    d = random(100, 300);
    fade(hsiNew[0], hsiNew[1], hsiNew[2], d);
    t += d;
    d = random(100, 300);
    delay(d);
    t += d;
  }
}

/*
  Fade from the current HSI state to the desired state over duration.

  Args:
    h/s/i: Desired end values.
    duration: Spread fade over this time duration in milliseconds.
*/
void HsiLight::fade(float h, float s, float i, uint16_t duration) {
  // Lower sleep equals smoother fade. Eg, hz = 1000 / sleep.
  uint16_t sleep = 25;  // ms
  uint16_t steps = duration / sleep;
  float inc[3];
  inc[0] = (h - _hsi[0]) / float(steps);
  inc[1] = (s - _hsi[1]) / float(steps);
  inc[2] = (i - _hsi[2]) / float(steps);
  // for (int i = 0; i < 3; i++) {
  //   inc[i] = (hsiNew[i] - _hsi[i]) / float(steps);
  // }
  for (int i = 0; i < steps; i++) {
    setHsi(_hsi[0] + inc[0],
           _hsi[1] + inc[1],
           _hsi[2] + inc[2]);
    delay(sleep);
  }
}

/* 
   Hue getter. 

   Using single variable getters as per Arduino style guide.
   https://www.arduino.cc/en/Reference/APIStyleGuide
*/
float HsiLight::getHue() {
  return _hsi[0];
}

/* Saturation getter. */
float HsiLight::getSaturation() {
  return _hsi[1];
}

/* Intensity getter. */
float HsiLight::getIntensity() {
  return _hsi[2];
}

/* 
   Convert HSI values to RGB values.
   
   Thanks to Brian Neltner for this code,
   http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi 

   Args:
     h/s/i: Desired hue/saturation/intensity
     rgb: 3-tuple array to fill in with resulting rgb values.
*/
void HsiLight::hsi2Rgb(float H, float S, float I, uint8_t rgb[]) {
  uint8_t r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if (H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } else if (H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  rgb[0]=r;
  rgb[1]=g;
  rgb[2]=b;
}

/*
  Private Methods
*/

void HsiLight::_setRgb(uint8_t rgb[]) {
  DEBUG_PRINT("Setting RGB: ");
  DEBUG_PRINT(rgb[0]); DEBUG_PRINT(", ");
  DEBUG_PRINT(rgb[1]); DEBUG_PRINT(", ");
  DEBUG_PRINT(rgb[2]); DEBUG_PRINT("\n");

  for (int i = 0; i < 3; i++)
    analogWrite(_rgbPins[i], rgb[i]);
}

