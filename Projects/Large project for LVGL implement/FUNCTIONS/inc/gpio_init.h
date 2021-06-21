#ifndef __GPIO_INIT_H

#define __GPIO_INIT_H
#include "sys.h"
void GPIO_Conf(void);
void Timer_PWM_Init(u32 arr, u16 psc);
void External_Interrupt_init(void);
void IWDG_Init(u8 prer, u32 rlr);
void WWDG_Init(u8 tr, u8 wr, u32 fprer); 
void General_Timer_Interrupt(u16 arr, u16 psc);
void TPAD_Reset(void);
u16 TPAD_Get_Val(void);
void CapacitiveTouch_Init(u16 psc);
u16 TPAD_Get_MaxVal(u8 sampt);
u8 TPAD_Scan(u8 mode);

#define LED00  GPIO_ResetBits(GPIOF, GPIO_Pin_9);
#define LED01  GPIO_SetBits(GPIOF, GPIO_Pin_9);
#define LED10  GPIO_ResetBits(GPIOF, GPIO_Pin_10);
#define LED11  GPIO_SetBits(GPIOF, GPIO_Pin_10);

#endif
