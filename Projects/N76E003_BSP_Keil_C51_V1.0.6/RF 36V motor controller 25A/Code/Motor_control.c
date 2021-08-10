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
////////////Variables////////////////
double bgvalue, ADCValue, bgvol, ADC_Vol;
uint8_t  bgh;
uint8_t  bgl;



void ADC_Init(void)
{
	set_IAPEN;
	IAPAL = 0x0c; IAPAH = 0x00; IAPCN = 0x04;
	set_IAPGO;
	bgh = IAPFD;
	IAPAL = 0x0d;
	IAPAH = 0x00; IAPCN = 0x04;
	set_IAPGO;
	bgl = IAPFD;
	bgl = bgl&0x0f; // lower four bits 
	bgvalue=(bgh<<4)+bgl;
	bgvol= bgvalue*3/4;
	printf("\nBandgap value:%x\n", bgvalue);
	printf("\nBandgap voltage:%dmV\n",bgvol);
	clr_IAPEN;		// turn off IAP
//	Enable_ADC_AIN4;		//P05 A_Det
//	Enable_ADC_AIN1;		//P30 Speed
}

UINT8 Get_HallValue(void)
{
	Enable_ADC_AIN0;			//P17 Hall pedal
	clr_ADCF;
	set_ADCS;
	ADCValue = (ADCRH<<4)+ADCRL;
	ADC_Vol = bgvol*ADCValue/bgvalue;
	printf("ADC_voltage:%gmV\n",ADC_Vol);//%g don't print no meaning 0
	if(ADC_Vol>1000)
	{
		return ADC_Vol;
	}
	else return 0;
}

void PWM_Init()
{
	PWM4_P01_OUTPUT_ENABLE;
	PWM5_P03_OUTPUT_ENABLE;
	PWM_COMPLEMENTARY_MODE;
	PWMPH = 0x01;  //Period setting;
	PWMPL = 0x83;	//14KHz
	/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/
	set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
	PWM4H = 0x03;						
	PWM4L = 0xCF;
	PWM5H = 0x05;						
	PWM5L = 0xCF;
	clr_SFRPAGE;
    set_LOAD;
    set_PWMRUN;	
}