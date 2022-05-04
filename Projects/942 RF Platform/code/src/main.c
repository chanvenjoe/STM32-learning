#include "main.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_Define.h"
#include "Common.h"
#include "Define.h"

SysClock_TypeDef tSysClock;



void main(void)
{
	MCU_Init();
	Flags_Init();
	
	while(1)
		{
				ADC_Refresh();
				
	
		}
}


 void Timer3_ISR (void) interrupt 16 
{
	  clr_TF3;
		SysClockCount();
}

static void SysClockCount(void)
{
	tSysClock.Tick500us++;

	if(tSysClock.Tick500us >10)	//
	{
				
				tSysClock.Tick500us = 0;
				tSysClock.Tick5ms++;
			if (tSysClock.ADCCount!=0)
		{
				tSysClock.ADCCount--;
		}
			if (tSysClock.UserCtrlCount!=0)
			{
				tSysClock.UserCtrlCount--;
			}
	}
}