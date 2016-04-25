#include <HSILight.h>

#define REDPIN 11
#define GREENPIN 10
#define BLUEPIN 9

// Add c++ style streaming IO support for easier printing.
template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg); return obj;
}

struct testVal {
  float hsi[3];
  uint8_t rgb[3];
};

const testVal testVals[3] = {
  { // red
    {0, 1, 1},   // hsi
    {254, 0, 0}  // rgb
  },
  { // green
    {120, 1, 1},  // hsi
    {0, 254, 0}   // rgb
  },
  { // blue
    {240, 1, 1},  // hsi
    {0, 0, 254}   // rgb
  }
};

HsiLight hsiLight(REDPIN, GREENPIN, BLUEPIN);

void setup() {
  Serial.begin(9600);
  uint8_t rgb[3];
  for (int color = 0; color < 3; color++) {
    hsiLight.hsi2Rgb(testVals[color].hsi[0],
                     testVals[color].hsi[1],
                     testVals[color].hsi[2],
                     rgb);
    if (rgb[0] != testVals[color].rgb[0] ||
        rgb[1] != testVals[color].rgb[1] ||
        rgb[2] != testVals[color].rgb[2]) {
      Serial << "FAILURE: HSI->RGB conversion\n";
      Serial << "Expected: " <<
        testVals[color].rgb[0] << "," <<
        testVals[color].rgb[1] << "," <<
        testVals[color].rgb[2] << "; " <<
        "Got: " << rgb[0] << "," << rgb[1] << "," << rgb[2] << "\n";
    }
  }
}

void loop() {
  Serial.println("red");
  hsiLight.setHsi(0, 1, 1);
  delay(2000);
  Serial.println("green");
  hsiLight.setHsi(120, 1, 1);
  delay(2000);
  Serial.println("blue");
  hsiLight.setHsi(240, 1, 1);
  delay(2000);
  Serial.println("fade to red");
  hsiLight.fade(0, 1, 1, 5000);
}