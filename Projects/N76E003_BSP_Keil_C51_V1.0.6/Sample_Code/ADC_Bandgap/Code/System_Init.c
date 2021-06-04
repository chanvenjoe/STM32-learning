#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

void Voltage_Check(void)
{
			long Voltage_Temp;
			clr_ADCF;
			set_ADCS;																	// Each time ADC start trig signal
      while(ADCF == 0);
			Voltage_Temp |= ADCRL;

}

void System_Init(void)
{
		Enable_ADC_AIN5; 															//Set P17 as ADC input
		Enable_ADC_BandGap;													//Find in "Function_define.h" - "ADC INIT"

}

void Self_Check(void)
{

		Voltage_Check();
}

void ADC_ISR (void) interrupt 11
{
		clr_ADCF;                               //clear ADC interrupt flag
        printf ("\n Value = 0x%bx",ADCRH);
}