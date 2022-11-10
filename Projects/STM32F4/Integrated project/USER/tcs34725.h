#ifndef _tsc__h
#define _tsc__h

#include "sys.h"

typedef struct{
	unsigned short  c;      //[0-65536]
	unsigned short  r;
	unsigned short  g;
	unsigned short  b;
}COLOR_RGBC;//RGBC

typedef struct{
	unsigned short h;       //[0,360]
	unsigned char  s;       //[0,100]
	unsigned char  l;       //[0,100]
}COLOR_HSL;//HSL

u8 TCS34725_Init(void);
u8 TCS34725_GetRawData(COLOR_RGBC *rgbc);


void RGBtoHSL(COLOR_RGBC *Rgb, COLOR_HSL *Hsl);
void delay_s(u32 i);
void TCS34725_I2C_Send_Byte(u8 byte);
u8 TCS34725_I2C_Wait_ACK(void);


#endif
