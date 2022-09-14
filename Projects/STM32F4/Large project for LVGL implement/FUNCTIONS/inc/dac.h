#ifndef _DAC__H
#define _DAC__H
#include "sys.h"

void MDAC_Init(void);
void DAC1_Set_Vol(u16 vol);

void ADC_DAC_Display(void);
void TIM9_PWM_Init(u16 arr, u16 psc);





#endif
