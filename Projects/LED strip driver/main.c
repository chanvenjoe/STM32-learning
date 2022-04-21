#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "WS2811.h"
#define T0H 	set_P00;		    //220-420
#define T0L 	clr_P00;clr_P00;clr_P00;clr_P00;clr_P00;//1000ns   750-1.6us
#define T1L 	clr_P00;		    //220-420
#define T1H 	set_P00;set_P00;set_P00;set_P00;set_P00;//1000ns   750-1.6us
#define CODE_0	T0H T0H
#define CODE_1	T1H T1L
#define RESET   clr_P00;Timer0_Delay125ns(15);set_P00;

WS_Hue_change();


void main()
{
	u8 i=1;
	System_init();
	while(1)
	{
		if(Get_HallValue()>100)
		{
			CODE_1
//			CODE_0
		}
		else
		{
			for(i=0;i<25;i++)
			{
				CODE_0
			}
			RESET
		}
				
//		WS_Hue_change();
//		if(i)
//		{
//			PWM2L++;
//			if(PWM2L==255)
//				i=0;
//			Ramp_up;
//		}
//		else
//		{
//			PWM2L--;
//			if(PWM2L==0)
//				i=1;
//			Ramp_up;
//		}
//		if(PWM1L<=255)
//		{
//			PWM1L++;
//			Ramp_up;
//		}
//		else
//		{
//			PWM2L--;
//			Ramp_up;
//		}
	}
}
