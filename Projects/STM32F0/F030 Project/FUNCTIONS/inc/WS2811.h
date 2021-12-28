#ifndef __WS2811_H
#define __WS2811_H
#include "stm32f0xx.h"
#include "delay.h"

#if 0
#define T0H GPIO_SetBits(GPIOF,GPIO_Pin_7);		delay_20ns(25);//500ns
#define T0L GPIO_ResetBits(GPIOF,GPIO_Pin_7); 		delay_20ns(100);//2us
#define T1H GPIO_SetBits(GPIOF,GPIO_Pin_7);		delay_20ns(100);
#define T1L GPIO_ResetBits(GPIOF,GPIO_Pin_7);		delay_20ns(25);
#define RES GPIO_ResetBits(GPIOF,GPIO_Pin_7);		delay_20ns(250); GPIO_SetBits(GPIOF,GPIO_Pin_7);
#endif

#define T0H GPIO_SetBits(GPIOF,GPIO_Pin_7);			__NOP();__NOP();__NOP();__NOP();__NOP();//500ns
#define T0L GPIO_ResetBits(GPIOF,GPIO_Pin_7); 		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();//2us  __NOP();=100ns
#define T1H GPIO_SetBits(GPIOF,GPIO_Pin_7);			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
#define T1L GPIO_ResetBits(GPIOF,GPIO_Pin_7);		__NOP();__NOP();__NOP();__NOP();__NOP();
#define RES GPIO_ResetBits(GPIOF,GPIO_Pin_7);		delay_us(50); GPIO_SetBits(GPIOF,GPIO_Pin_7);

#define TEST_delay delay_20ns(1)
#define CODE_0 T0H T0L
#define CODE_1 T1H T1L
#define LEDNUM 		10

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

void WS_Send24bits(u32 color);
void WS_ColorSet_LED(u8 from, u8 to, u32 color);
void WS_Color_copy(u8 No, u32 color);
void WS_Refresh(void);
void WS_Key_RGB(void);
void TPAD_Reset(void);
u16 TPAD_Get_Val(void);
u32 CapacitiveTouch_Init(u16 psc);
u16 TPAD_Get_MaxVal(u8 sampt);
u8 TPAD_Scan(u8 mode,u32 tpad_average1);
void Tpad_IOInit(u16 psc);
#endif
