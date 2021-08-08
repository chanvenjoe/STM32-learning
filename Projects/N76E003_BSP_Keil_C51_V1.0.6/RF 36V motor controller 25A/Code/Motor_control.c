#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"

uint16_t bgvalue;
uint8_t  bgh;
uint8_t  bgl;
static uint16_t ADCValue;
static uint16_t ADC_Vol;

void ADC_Init(void)
{
	UINT8 i;
	Enable_ADC_BandGap;													//Find in "Function_define.h" - "ADC INIT"
	Enable_ADC_AIN4;		//P05 A_Det
	Enable_ADC_AIN0;			//P17 Hall pedal
	Enable_ADC_AIN1;		//P30 Speed

	for(i=0;i<3;i++)
	{
		clr_ADCF;
		set_ADCS;
		printf ("\n Value = 0x%bx",ADCRH);
		printf ("\n Value = 0x%bx",ADCRL);
		bgh=ADCRH;
		bgl=ADCRL;
		bgvalue = bgh<<4+bgl;
		printf ("\n BGVALUE = %d",bgvalue);
		while(ADCF==0);		
	}
	
	bgh = ADCRH;
	bgl = ADCRL;
	ADCValue = bgh<<4+bgl;
	ADC_Vol = 1.22*ADCValue/bgvalue;
	if(ADC_Vol>1)
	{
		i = (ADC_Vol-1)%3;
	}
}


