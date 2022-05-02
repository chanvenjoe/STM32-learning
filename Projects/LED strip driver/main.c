#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"
#include "WS2811.h"




void main(void)  
{
	System_init();
	P03_Quasi_Mode;
	clr_P03;
	while(1)
	{
		switch((P03)==1)
		{
			case 0:
			{
				WS_Hue_change();
			}
			break;
			case 1:
			{
				WS_voice_Pik(1);
			}
			break;
			default:
				break;
		}
		
	}
}
