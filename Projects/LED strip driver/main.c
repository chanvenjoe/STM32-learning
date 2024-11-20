#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "WS2811.h"

u8 flag=0;
u8 key_val;

void main(void)
{
	System_init();
	P03_Quasi_Mode;
	clr_P03;
	while(1)
	{
//		set_WDCLR;
		if(P03==1)
		{
			Timer1_Delay10ms(5);
			if(P03==1)
			{
				flag=~flag;
			}
		}
		switch(P03==0)
		{
			case 0:
			{
				WS_voice_Pik(1);
			}
			break;
			case 1:
			{
				WS_Hue_change();
			}
			break;
			default:
				break;
		}
		
	}
}
