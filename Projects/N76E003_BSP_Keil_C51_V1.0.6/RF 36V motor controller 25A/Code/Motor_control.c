#include "Motor_control.h"

void ADC_Init(void)
{
		Enable_ADC_BandGap;													//Find in "Function_define.h" - "ADC INIT"
		Enable_ADC_AIN4;		//P05 A_Det
		Enable_ADC_AIN0;			//P17 Hall pedal
		Enable_ADC_AIN1;		//P30 Speed
		for(u8 i=0;i<3;i++)
		{
			clr ADCF

