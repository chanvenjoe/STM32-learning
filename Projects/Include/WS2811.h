#ifndef __WS2811_H
#define __WS2811_H

#include "Delay.h"
#include "Function_define.h"
#include "SFR_Macro.h"

#define NOP_3	_nop_();_nop_();_nop_();_nop_();// 270ns
#define NOP_9	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();// 800ns
#define T0H 	set_P00;NOP_3//		220-420
#define T0L 	clr_P00;NOP_9//  	750-1000
#define T1L 	clr_P00;NOP_3//		220-420
#define T1H 	set_P00;NOP_9//		750-1.6us
#define CODE_0	T0H T0L
#define CODE_1	T1H T1L
#define RES		clr_P00;Timer0_Delay125ns(3);set_P00;
#define LEDNUM 		64
#define LEDARR		8
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
#define Blue        0x00001f
#define Test		0xFFDEAD
#define cmp = 		0x00800000;
#define dummy		0x000000
#define WS2811		0
#define WS2812B		1
#define LED_type	WS2812B
#define FadeoutTime	5
#define HSV_Ch_Sp	5
#define HSV_Delay1ms  	10
#define LED_Number_mode 1

void WS_Send24bits(u32 color);
void WS_ColorSet_LED(u8 from, u8 to, u32 color);
void WS_Color_copy(u8 No, u32 color);
void WS_Refresh(void);
void WS_Key_RGB(void);
void WS_Hue_change(void);
void WS_voice_Pik(u8 mode);
u8	Get_RGB_Max(u8 R, u8 G, u8 B);
u8	Get_RGB_Min(u8 R, u8 G, u8 B);
u8	Get_Hue_Val(u8 R, u8 G, u8 B);
u32 HSV_RGB(int h, char s, char v, float R, float G, float B);

enum Hue_formular
{
	Non=0,First, Second, Third, Fourth, Fifth
};

static int h=0;

#endif