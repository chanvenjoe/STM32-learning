#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************

/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/

void main (void)
{
	System_init();
	while(1)
	{
		Movement_control();
	}
}





