#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"



void main()
{
	u8 i=1;
	System_init();
	while(1)
	{
		if(i)
		{
			PWM2L++;
			if(PWM2L==255)
				i=0;
			Ramp_up;
		}
		else
		{
			PWM2L--;
			if(PWM2L==0)
				i=1;
			Ramp_up;
		}
		if(PWM1L<=255)
		{
			PWM1L++;
			Ramp_up;
		}
		else
		{
			PWM2L--;
			Ramp_up;
		}
	}
}
