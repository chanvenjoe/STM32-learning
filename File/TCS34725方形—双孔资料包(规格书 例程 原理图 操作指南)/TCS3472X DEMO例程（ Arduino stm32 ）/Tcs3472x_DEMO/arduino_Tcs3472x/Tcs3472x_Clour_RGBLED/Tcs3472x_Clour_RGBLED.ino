/*接线方式 */
/*
RGB灯环    Arduino_Uno 
  DIN        PIN 6     
  VCC         5V
  GND         GND
  
TCS3473x  Arduino_Uno 
  SDA         A4
  SCL         A5
  VIN         5V
  GND         GND
*/
#include <Wire.h>
#include "MH_TCS34725.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// our RGB -> eye-recognized gamma color
byte gammatable[256];

MH_TCS34725 tcs = MH_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  Serial.println("Color View Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  pixels.begin(); // This initializes the NeoPixel library.
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    gammatable[i] = x;
  }
}

void loop() {
  uint16_t clear, red, green, blue;
  tcs.getRGBC(&red, &green, &blue, &clear);
  tcs.lock();  // turn off LED
  
  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
  Serial.print("\t");
  // Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  Serial.print((int)r); Serial.print("\t"); 
  Serial.print((int)g); Serial.print("\t");
  Serial.print((int)b);
  Serial.println();

  //Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );
  //Set the color lamp
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b])); // Moderately bright green color.
    // pixels.setPixelColor(i, pixels.Color((int)r, (int)g, (int)b));

    pixels.show(); // This sends the updated pixel color to the hardware.

    // delay(500); // Delay for a period of time (in milliseconds).
  }
}
