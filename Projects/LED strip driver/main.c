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
	key_val=P03;
	while(1)
	{
		if(P03!=key_val)
		{
			Timer1_Delay10ms(5);
			if(P03!=key_val)
			{
				flag=~flag;
				key_val=P03;
			}
		}
		switch(flag)
		{
			case 0:
			{
				WS_Hue_change();
			}
			break;
			case 255:
			{
//				WS_ColorSet_LED(0, 64, Black);
//				WS_ColorSet_LED(0, 3, Blue);
//				WS_Refresh();
				WS_voice_Pik(1);
			}
			break;
			default:
				break;
		}
		
	}
}
