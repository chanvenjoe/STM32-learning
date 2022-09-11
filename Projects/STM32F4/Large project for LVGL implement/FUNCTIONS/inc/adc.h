#ifndef __adc_h
#define __adc_h
#include "sys.h"

void ADC_Init_Config(void);
u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch);
u16 Get_ADC_Average(ADC_TypeDef* ADCx,u8 ch, u8 avg_num);


#endif

