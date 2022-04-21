#ifndef __WS2811_H
#define __WS2811_H
#include "Delay.h"
#include "common.h"
//#include "Motor_control.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#define T0H 	set_P00;	Timer0_Delay125ns(3);//375ns
#define T0L 	clr_P00; 	Timer0_Delay125ns(8);//1000ns
#define T1H 	set_P00;	Timer0_Delay125ns(8);
#define T1L 	clr_P00;	Timer0_Delay125ns(3);
#define RES 	clr_P00;	Timer0_Delay100us(5);
#define TEST_delay Timer0_Delay125ns(1);
#define CODE_0 T0H T0L
#define CODE_1 T1H T1L
#define LEDNUM 		10
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
typedef struct color
{
	unsigned long RGB; 
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

//void TPAD_Reset(void);
//u16 TPAD_Get_Val(void);
//u32 CapacitiveTouch_Init(u16 psc);
//u16 TPAD_Get_MaxVal(u8 sampt);
//u8 TPAD_Scan(u8 mode,u32 tpad_average1);
//void Tpad_IOInit(u16 psc);

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
