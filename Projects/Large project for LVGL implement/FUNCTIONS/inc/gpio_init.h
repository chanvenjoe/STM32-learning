#ifndef __GPIO_INIT_H

#define __GPIO_INIT_H
#include "sys.h"
void GPIO_Conf(void);
void Timer_PWM_Init(u32 arr, u16 psc);
void External_Interrupt_init(void);
void IWDG_Init(u8 prer, u32 rlr);
#endif
