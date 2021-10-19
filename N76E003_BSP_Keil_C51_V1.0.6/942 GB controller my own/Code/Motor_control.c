#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Delay.h" 
#include "SFR_Macro.h"

#define Vref  3072;
#define set_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=SET_BIT0 ;EA=BIT_TMP
#define set_IAPGO BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0 ;EA=BIT_TMP
#define clr_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;EA=BIT_TMP
////////////Variables////////////////
double bgvalue, ADCValue, bgvol, ADC_Vol;
uint8_t  bgh;
uint8_t  bgl;

// PWM+=KP[e(k) -e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
// e(k) the value difference of actual and setting e(k-1) the last time difference
// In this motor control we use P
// PWM = Kp[e(k) - e(k-1)]

UINT8 Incremental_P(UINT8 Cbat, UINT8 CC_20)
{
	float Kp=20;
	static int Bias, PWM,Last_bias;
	Bias= Cbat-CC_20;
	PWM-=Kp*(Bias-Last_bias);// Decreasement output
	Last_bias = Bias;
	return PWM;
}
	
	

void ADC_Init(void)
{
	set_EA;
	set_EPI;//Enable Pin interrup
	P13_Input_Mode;
	
	clr_PIPS1;		//Power switch
	clr_PIPS0;//pin1. pin 
	set_PIT3;//cHANNEL 3 Edge triggered
	clr_PIPEN3;//P13 falling triggered
	set_PINEN3;
	
	P14_PushPull_Mode;
	P00_PushPull_Mode;
	set_P14; //Enable DCDC
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
	Enable_ADC_AIN0;		//P17 Hall pedal
//	Enable_ADC_AIN4;		//P05 A_Det
//	Enable_ADC_AIN1;		//P30 Speed
	for(bgh =0;bgh<3;bgh++)
	{
		clr_ADCF;
		set_ADCS;
		ADCValue = (ADCRH<<4)+ADCRL;
		Timer0_Delay1ms(10);
	}
}

UINT16 Get_HallValue(void)
{
	Enable_ADC_AIN0;
	clr_ADCF;
	set_ADCS;
	ADCValue = (ADCRH<<4)+ADCRL;
	printf("ADC value:%d",ADCValue);
	ADC_Vol = (bgvol*ADCValue/bgvalue);//All are decimal
	printf("ADC_voltage:%gmV\n",ADC_Vol);//%g don't print no meaning 0
	if(ADC_Vol>1000)
	{
		return ADC_Vol;
	}
	else return 0;
}

UINT16 Get_CurrentValue(void)
{
	Enable_ADC_AIN4;
	clr_ADCF;
	set_ADCS;
	ADCValue = (ADCRH<<4)+ADCRL;
	printf("ADC value:%d",ADCValue);
	return ADCValue;
}

void PWM_Init()
{
	PWM5_P03_OUTPUT_ENABLE;
	PWM4_P01_OUTPUT_ENABLE;//Upper bridge
	PWM_CLOCK_DIV_8;
	PWM_IMDEPENDENT_MODE;
	PWM4_OUTPUT_INVERSE;
	PWMPH = 0x00;  //Period setting;
	PWMPL = 0x96;	//14KHz
	PWM5H = 0x00;
	PWM5L = 0x00;
	PWM4H = 0x00;
	PWM4L = 0x00;
	/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/
}

void PWM_Setting(UINT16 n)	//1n = 1%
{
	set_SFRPAGE; //PWM4\5 SETTING
//	printf("ADC value:%d\n",ADCValue);
//	printf("ADC_voltage:%gmV\n",ADC_Vol);/
	if(n>100)
	{
		PWM4L = 0x97;
	}
	else if(n==0)
	{
		PWM4L = 0X00;
	}
		//PWM4L = 0X00;  //Upper bridge set to high when pedal lower than 1.0V
	else
	{
		PWM4L = (n*3/2);
	}
	
	PWM5H = PWM4H;
	PWM5L = PWM4L;

 	set_LOAD;
	set_PWMRUN;
}

void GPIO_Init()
{
	set_P13;//LED on
	set_P14; //Enable the DCDC
	P05_Input_Mode; //Current sensing
	
}

//void PinInterrupt (void) interrupt 7
//{
//	printf("Falling edge intterrupt triggered");
//}