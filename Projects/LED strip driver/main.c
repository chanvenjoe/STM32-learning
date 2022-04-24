#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "WS2811.h"




void main(void)  
{
	u8 dB;
	u32 color=0;
	System_init();
	while(1)
	{
		dB = Get_HallValue();
		if(dB>=30&&dB<125)
		{
			
			color = Black|(dB*2);
			WS_ColorSet_LED(0,10,color);
			WS_Refresh();
			Timer1_Delay10ms(5);
//			while(color)
//			{
//				color-=5;
//				WS_ColorSet_LED(0,10,color);
//				WS_Refresh();
//			}
		}
		else if(dB>=125)
		{
			color = Green;
			WS_ColorSet_LED(0,10,color);
			WS_Refresh();
			Timer1_Delay10ms(5);
		}
		else
		{
			WS_ColorSet_LED(0,10,0x000000);
			WS_Refresh();
			Timer1_Delay10ms(5);			
		}

	}
}
