/*接线方式 */
/*
0.95RGB_oled    Arduino_Uno 
   sclk             13
   mosi             11
   cs               10
   rst              9
   dc               8
  VCC         		5V
  GND         		GND
  
TCS3473x  Arduino_Uno 
  SDA         A4
  SCL         A5
  VIN         5V
  GND         GND
  LED          6
  
  button
  TCS3473x  Arduino_Uno 
    BUT           2
*/
#include <Adafruit_GFX.h>
//#include<Adafruit_SPITFT.h>
#include <Adafruit_SSD1331.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif


//0.96RGB 引脚定义
#define sclk 13
#define mosi 11
#define cs  10
#define rst  9
#define dc   8

#define BUT 2
#define LED 6

#define CALIBRATION true

//颜色定义
#define BLACK      0X0000
#define BLUE       0X001F
#define RED        0XF800
#define GREEN      0X07E0
#define CYAN       0X07FF
#define MAGENTA    0XF81F
#define YELLOW     0XFFE0
#define WHITE      0XFFFF
#define GREY       0XC618

//TCS3472x积分时间，增益值控制
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
//板载LED亮度调节

/*
RGB值过大，光强超出检查范围，减小增益（TCS34725_GAIN_16X)

*/



// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// our RGB -> eye-recognized gamma color
byte gammatable[256];

//DFRobot_TCS34725 tcs = DFRobot_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//RGB赋初值
int R = 0;
int G = 0;
int B = 0;
//最大最小值赋初值
int minR,minG,minB = 255;
int maxR,maxG,maxB = 0;

//定义阈值
int minThreshold = 5;
int calibState = 0; //0 == black; 1 == white, 2 == done;

int lightVal;


//初始化RGB_OLED
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
 {
  Serial.begin(9600);
  Serial.println("Color OLED Test!");

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  pinMode(BUT,INPUT); // set button GPIO INPUT.
  pinMode(LED,OUTPUT); // set button GPIO INPUT.


  
//oled begin display ,clear to black
  display.begin();
  display.fillScreen(BLACK);
  display.setCursor(0,0);

  
  if(CALIBRATION)
  {
	//Start calibration at black
	calibState = 0;
	display.println("calibrate");
	display.print("Black...");
  }
  else
  {
	minR = minG = minB = 0;
	maxR = maxG = maxB = 255;
  } 
  
 /* for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    gammatable[i] = x;
  }*/
}

void DoButton()
{
	if(calibState == 0)
	{
		//
		minR = 0;
		minG = 0;
		minB = 0;
		
		display.fillScreen(BLACK);
		display.setCursor(0,0);
		display.println("calibrate");
		display.print("white...");
		
		calibState = 1;
	}
	else if(calibState == 1)
	{
		//
		maxR = R;
		maxG = G;
		maxB = B;
		
		display.fillScreen(BLACK);
		display.setCursor(0,0);
		
		calibState = 2;
	}
}

void loop() {
  int val = analogRead(0);
  lightVal = map(val, 0, 1023, 0, 255);
  analogWrite(LED,lightVal);
  
  uint16_t clear, red, green, blue;  
  tcs.setInterrupt(false);
  delay(60);
  tcs.getRawData(&red, &green, &blue, &clear);
  tcs.setInterrupt(true);
  
  //tcs.lock();  // turn off LED
  
  // Figure out some basic hex code for visualization
  uint32_t sum = clear;
  float r, g, b;
  
  r = red;
  r /= sum;
  
  g = green;
  g /= sum;
  
  b = blue;
  b /= sum;
  
  r *= 256;
  g *= 256;
  b *= 256;
  
  R = r;
  G = g;
  B = b;
  
 /* Serial.print("\t");
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
  }*/
  if(digitalRead(BUT) == HIGH)
  {
	  DoButton();
  }
  PrintRGB();
  if(CALIBRATION)
  {
	  if(calibState < 2)
	  {
		  display.fillRect(0,display.height()-20,display.width(),10,BLACK);
		  display.setCursor(0,display.height()-20);
		  display.setTextColor(WHITE);
		  display.setTextSize(1);
		  display.print("RGB");
		  display.print(R);
		  display.print(",");
		  display.print(G);
		  display.print(",");
		  display.print(B);
		  display.print(",");
		  
		  
		  return;
	  }
  }
  
  int R2 = map(R,minR,maxR,0,255 );
  int G2 = map(G,minG,maxG,0,255 );
  int B2 = map(B,minB,maxB,0,255 );
  
  R = constrain(R2,0,255);
  G = constrain(G2,0,255);
  B = constrain(B2,0,255);
  
  int stateColor = -1;
  
  if(R > B + minThreshold && R > G + minThreshold)
	  stateColor = 0;
  if(G > R + minThreshold && G > B + minThreshold)
	  stateColor = 1;
  if(B > R + minThreshold && B > R + minThreshold)
	  stateColor = 2;
  
  int colo = convertRGB24toRGB565(R,G,B);
  
  //Display RGB data on screen
  display.fillRect(0,0,display.width(),10,BLACK);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print("RGB");
  display.print(R);
  display.print(",");
  display.print(G);
  display.print(",");
  display.print(B);
  display.print(",");
  
  
  display.fillRect((display.width()-1)/2-40,(display.height()-1)/2-10,80,80,colo);
 /* 
  if(stateColor == -1)
	display.fillRect((display.width()-1)-20,(display.height()-1)/2-10,20,20,BLACK);
  else if(stateColor == 0)
	display.fillRect((display.width()-1)-20,(display.height()-1)/2-10,20,20,RED);
  else if(stateColor == 1)
	display.fillRect((display.width()-1)-20,(display.height()-1)/2-10,20,20,GREEN);
  else if(stateColor == 2)
	display.fillRect((display.width()-1)-20,(display.height()-1)/2-10,20,20,BLUE);
	*/  
}
void PrintRGB()
{
	Serial.print("RGB[");
	Serial.print(R);
	Serial.print(",");
	Serial.print(G);
	Serial.print(",");
	Serial.print(B);
	Serial.print("]");
	
	Serial.print("MIN[");
	Serial.print(minR);
	Serial.print(",");
	Serial.print(minG);
	Serial.print(",");
	Serial.print(minB);
	Serial.print("]");

	Serial.print("MAX[");
	Serial.print(maxR);
	Serial.print(",");
	Serial.print(maxG);
	Serial.print(",");
	Serial.print(maxB);
	Serial.println("]");	
}
  

uint16_t convertRGB24toRGB565(uint8_t r,uint8_t g,uint8_t b)
{
  return ((r/8)<<11)|((g/4)<<5)|(b/8);
}
