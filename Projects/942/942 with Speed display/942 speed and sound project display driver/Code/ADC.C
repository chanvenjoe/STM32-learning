#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "Speed_display.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#define CCvalue 0x14; //change the current regulation value

#define Not_Pressed PWM5_P03_OUTPUT_DISABLE; PWM4_P01_OUTPUT_DISABLE; clr_P01; set_P03;
#define Pressed PWM5_P03_OUTPUT_ENABLE; PWM4_P01_OUTPUT_ENABLE;
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
<<<<<<< Updated upstream
float IC0_value;
unsigned int IC0_value_sum;
void Capture_ISR(void) interrupt 12  //should sample the Value 1s a time, which doesn't take too much resource of MCU
{
	clr_CAPF0;
	IC0_value = (C0H<<8)+C0L; //uS value 
	//IC0_value = (1/(IC0_value*2/1000000))*0.05;//3600/60.67*3.14*0.27/1000;
	IC0_value_sum+=IC0_value;
	clr_TF2;
}
=======
int IC0_value[10];
void Capture_ISR(void) interrupt 12
{
	u8 i=0;
	clr_CAPF0;//Clear capture flag
	if(++i<=10) i=0;
	IC0_value[i] = (C0H<<4)||C0L;
}

>>>>>>> Stashed changes
void main (void) 
{
	Set_All_GPIO_Quasi_Mode;			//For GPIO1 output, Find in "Function_define.h" - "GPIO INIT"
	P00_PushPull_Mode;
	P01_PushPull_Mode;
	P02_PushPull_Mode;
	P03_PushPull_Mode;
	P04_PushPull_Mode;
	P05_PushPull_Mode;
	P06_PushPull_Mode;
	P07_PushPull_Mode;
	P10_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;
	P13_PushPull_Mode;
	InitialUART0_Timer1(115200);
	
	P15_Input_Mode;
	P15=1;
	
	TIMER2_CAP0_Capture_Mode;
	TIMER2_DIV_16;
	//IC7_P15_CAP0_RisingEdge_Capture;
	//IC7_P15_CAP0_BothEdge_Capture;
	IC7_P15_CAP0_FallingEdge_Capture;
	set_ECAP;
	set_TR2;
	set_EA;
	ADC_Init();							//
										//reverved for timer_init   Sleep2
//	PWM_Init();
	while(1)
	{
		float IC0_value_show = IC0_value;
		float IC0_value_last;
		
		IC0_value_show = 1000000/IC0_value_show;
//		IC0_value_show = (1/(IC0_value_show*2/1000000))*0.05;//3600/60.67*3.14*0.27/1000;
		if(IC0_value_show>0&&IC0_value_show<0xfff)
		{
			numb_show(IC0_value_show);
		}
		else
		{
			numb_show(0);
		}
	}
}


