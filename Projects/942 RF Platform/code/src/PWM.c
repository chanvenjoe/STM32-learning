#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_Define.h"
#include "Common.h"
#include "Define.h"

void PWM_Set(UINT8 duty){
		set_SFRPAGE; //PWM4\5 SETTING
		PWM4H = (0xff00&duty)>>8;//Lower bridge P01
		PWM4L = 0x28+(0xff&duty)*2;
		PWM5H = PWM4H;
		PWM5L = PWM4L;
		set_LOAD;
		set_PWMRUN;
}

UINT8 PWM_Get(UINT16 ThrottleVol){
		UINT8 duty;
		duty = (ThrottleVol-1000)/28;
		return duty;
}



	
		
