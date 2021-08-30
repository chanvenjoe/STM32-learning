#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "Delay.h"

#define threshold 1000;
//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#if 0
//#define Enable_ADC_AIN0			ADCCON0&=0xF0;P17_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT0;ADCCON1|=SET_BIT0									//P17
//#define Enable_ADC_AIN1			ADCCON0&=0xF0;ADCCON0|=0x01;P30_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT1;ADCCON1|=SET_BIT0		//P30
//#define Enable_ADC_AIN2			ADCCON0&=0xF0;ADCCON0|=0x02;P07_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT2;ADCCON1|=SET_BIT0		//P07
//#define Enable_ADC_AIN3			ADCCON0&=0xF0;ADCCON0|=0x03;P06_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT3;ADCCON1|=SET_BIT0		//P06
//#define Enable_ADC_AIN4			ADCCON0&=0xF0;ADCCON0|=0x04;P05_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT4;ADCCON1|=SET_BIT0		//P05
//#define Enable_ADC_AIN5			ADCCON0&=0xF0;ADCCON0|=0x05;P04_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT5;ADCCON1|=SET_BIT0		//P04
//#define Enable_ADC_AIN6			ADCCON0&=0xF0;ADCCON0|=0x06;P03_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT6;ADCCON1|=SET_BIT0		//P03
//#define Enable_ADC_AIN7			ADCCON0&=0xF0;ADCCON0|=0x07;P11_Input_Mode;AINDIDS=0x00;AINDIDS|=SET_BIT7;ADCCON1|=SET_BIT0		//P11
//#define Enable_ADC_BandGap	ADCCON0|=SET_BIT3;ADCCON0&=0xF8;																															//Band-gap 1.22V

//#define PWM0_FALLINGEDGE_TRIG_ADC		ADCCON0&=~SET_BIT5;ADCCON0&=~SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1&=~SET_BIT2;ADCCON1|=SET_BIT1
//#define PWM2_FALLINGEDGE_TRIG_ADC		ADCCON0&=~SET_BIT5;ADCCON0|=SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1&=~SET_BIT2;ADCCON1|=SET_BIT1
//#define PWM4_FALLINGEDGE_TRIG_ADC		ADCCON0|=SET_BIT5;ADCCON0&=~SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1&=~SET_BIT2;ADCCON1|=SET_BIT1
//#define PWM0_RISINGEDGE_TRIG_ADC		ADCCON0&=~SET_BIT5;ADCCON0&=~SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1|=SET_BIT2;ADCCON1|=SET_BIT1
//#define PWM2_RISINGEDGE_TRIG_ADC		ADCCON0&=~SET_BIT5;ADCCON0|=SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1|=SET_BIT2;ADCCON1|=SET_BIT1
//#define PWM4_RISINGEDGE_TRIG_ADC		ADCCON0|=SET_BIT5;ADCCON0&=~SET_BIT4;ADCCON1&=~SET_BIT3;ADCCON1|=SET_BIT2;ADCCON1|=SET_BIT1

//#define P04_FALLINGEDGE_TRIG_ADC		ADCCON0|=0x30;ADCCON1&=0xF3;ADCCON1|=SET_BIT1;ADCCON1&=~SET_BIT6
//#define P13_FALLINGEDGE_TRIG_ADC		ADCCON0|=0x30;ADCCON1&=0xF3;ADCCON1|=SET_BIT1;ADCCON1|=SET_BIT6
//#define P04_RISINGEDGE_TRIG_ADC			ADCCON0|=0x30;ADCCON1&=~SET_BIT3;ADCCON1|=SET_BIT2;ADCCON1|=SET_BIT1;ADCCON1&=~SET_BIT6
//#define P13_RISINGEDGE_TRIG_ADC			ADCCON0|=0x30;ADCCON1&=~SET_BIT3;ADCCON1|=SET_BIT2;ADCCON1|=SET_BIT1;ADCCON1|=SET_BIT6
#endif

/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/

void main (void) 
{
	Set_All_GPIO_Quasi_Mode;				//For GPIO1 output, Find in "Function_define.h" - "GPIO INIT"
	InitialUART0_Timer1(115200);
	ADC_Init();							//
										//reverved for timer_init   Sleep
	PWM_Init();
	while(1)
	{
		UINT16 i = Get_HallValue();
		if(i>1000)
		{
			UINT16 pwm_step = (i-0x3e8)/0x1E; //1.0->4.0
			set_P00;		//Relay open
	//		printf("PWM:%d %",pwm_step);
			PWM_Setting(pwm_step);
	//			Timer0_Delay1ms(10);
		}
		else
		{
			PWM_Setting(0x00);
			clr_P00;
		}
			

	}
}


