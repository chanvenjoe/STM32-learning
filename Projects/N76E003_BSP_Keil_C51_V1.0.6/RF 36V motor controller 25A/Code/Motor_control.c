#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Delay.h"

#define Vref  3072;
#define set_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=SET_BIT0 ;EA=BIT_TMP
#define set_IAPGO BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0 ;EA=BIT_TMP
#define clr_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;EA=BIT_TMP

uint32_t bgvalue;
static uint16_t bgvol;
uint32_t temp;
uint16_t  bgh;
uint16_t  bgl;
static uint16_t ADCValue;
static double ADC_Vol;


void ADC_Init(void)
{
	Enable_ADC_BandGap;													//Find in "Function_define.h" - "ADC INIT"
	set_IAPEN;
	IAPAL = 0x0c; IAPAH = 0x00; IAPCN = 0x04;
	set_IAPGO;
	bgh = IAPFD;
	IAPAL = 0x0d;
	IAPAH = 0x00; IAPCN = 0x04;
	set_IAPGO;
	bgl = IAPFD;
	bgl = bgl&0x0f; // lower four bits 
	clr_IAPEN;		// turn off IAP
	bgvalue=(bgh<<4)+bgl;
	temp = 0xc00*bgvalue;
	temp = temp/0x1000;
	bgvol = temp;
//	printf("\nBandgap value:%x\n", bgvalue);
	printf("\nBandgap voltage:%dmV\n",bgvol);
//	Enable_ADC_AIN4;		//P05 A_Det
//	Enable_ADC_AIN1;		//P30 Speed
}

//UINT8 Get_HallValue()
//{
//	Enable_ADC_AIN0;			//P17 Hall pedal
//	clr_ADCF;
//	set_ADCS;
//	ADCValue = ADCRH<<4|ADCRL;
// 	ADC_Vol = 1.2 * ADCValue/bgvalue;
//	if(ADC_Vol>1)
//	{
//		UINT8 i = ADCValue/34;
//		return i;
//	}
//	return 0;
//}
