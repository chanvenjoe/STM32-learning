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
	while(1)
	{
//		while(Get_HallValue()>50)
//			WS_Hue_change();
		WS_voice_Pik(1);
	}
}
