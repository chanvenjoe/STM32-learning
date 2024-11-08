#ifndef __WS2811_H
#define __WS2811_H
#include "sys.h"
#include "delay.h"

#define T0H 	GPIO_SetBits(GPIOF,GPIO_Pin_6);		delay_47_6ns(3);//500ns
#define T0L 	GPIO_ResetBits(GPIOF,GPIO_Pin_6); 	delay_47_6ns(34);//2us
#define T1H 	GPIO_SetBits(GPIOF,GPIO_Pin_6);		delay_47_6ns(34);
#define T1L 	GPIO_ResetBits(GPIOF,GPIO_Pin_6);	delay_47_6ns(3);
#define RES 	GPIO_ResetBits(GPIOF,GPIO_Pin_6);	delay_47_6ns(1300); GPIO_SetBits(GPIOF,GPIO_Pin_6);
#define TEST_delay delay_47_6ns(1)
#define CODE_0 T0H T0L
#define CODE_1 T1H T1L
#define LEDNUM 		3

typedef struct color
{
	u32 RGB; 
	u8 R;
	u8 G;
	u8 B;
}Color_Typedef;

#define White       0xFFFFFF
#define Black       0x000000
#define Green       0x00ff00
#define Red	        0xff0000
#define Blue        0x0000ff
#define cmp = 		0x00800000;
#define dummy		0x000000

void WS_Send24bits(u32 color);
void WS_ColorSet_LED(u8 from, u8 to, u32 color);
void WS_Color_copy(u8 No, u32 color);
void WS_Refresh(void);
void WS_Key_RGB(void);
void WS_Hue_change(void);
u8	Get_RGB_Max(u8 R, u8 G, u8 B);
u8	Get_RGB_Min(u8 R, u8 G, u8 B);
u8	Get_Hue_Val(u8 R, u8 G, u8 B);
u32 HSV_RGB(int h, char s, char v, float R, float G, float B);

enum Hue_formular
{
	Non=0,First, Second, Third, Fourth, Fifth
};

//struct
//{
//	u16 h;
//	float s;
//	float v;
//}HSV;

#endif
