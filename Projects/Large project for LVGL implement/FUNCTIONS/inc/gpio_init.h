#ifndef __GPIO_INIT_H

#define __GPIO_INIT_H
#include "sys.h"
void GPIO_Conf(void);
void Timer_PWM_Init(u32 arr, u16 psc);
void External_Interrupt_init(void);
void IWDG_Init(u8 prer, u32 rlr);
void WWDG_Init(u8 tr, u8 wr, u32 fprer); 
void General_Timer_Interrupt(u16 arr, u16 psc);

#define LED00  GPIO_ResetBits(GPIOF, GPIO_Pin_9);
#define LED01  GPIO_SetBits(GPIOF, GPIO_Pin_9);

#endif
