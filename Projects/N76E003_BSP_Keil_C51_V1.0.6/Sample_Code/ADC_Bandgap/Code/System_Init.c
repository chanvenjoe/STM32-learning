#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"

UINT8 Voltage_Check(void)
{
			int c;
			long Voltage_Temp;
			clr_ADCF;
			set_ADCS;																	// Each time ADC start trig signal
      while(ADCF == 0);
			Voltage_Temp = ADCRH * 4 + ADCRL;
			if (Voltage_Temp < 0x40)
				{
					c=1;
				}
			else if (Voltage_Temp > 0x80)
				{
					c=2;
				}
			else
				c=0;
			return c;


}

void System_Init(void)
{
		Enable_ADC_AIN0; 															//Set P17 as ADC input
		Enable_ADC_BandGap;													//Find in "Function_define.h" - "ADC INIT"

}

void Self_Check(void)
{

		Voltage_Check();
}
