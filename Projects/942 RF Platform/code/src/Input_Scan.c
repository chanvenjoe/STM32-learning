#include "Input_Scan.h"


/*********************************************
Function: getGearSelected;
DESC: determine the selected gear from ADC reading;
Input: ADC value;
Output: Changed gear setting in tUserCtrl.Gear;
Return: NA;
**********************************************/
static void getGearSelected(UINT16 GearInput){
			UINT16 Temp;
			Temp=tSysClock.UserCtrlCount;
			if (Temp != tUserCtrl.Count)	//access the function in every 5ms
			{
					Temp	= tUserCtrl.Count;
					getGearSelectCount++;			//delay 15ms
					if (getGearSelectCount>2)
					{	
							UINT16 inputTemp;
				
							getGearSelectCount=0;
							tSysClock.UserCtrlCount=0;
						
							if (inputTemp!=GearInput)
							inputTemp = GearInput;
							if ( inputTemp>>GEARREVLO && inputTemp<<GEARREVHI){
									tUserCtrl.Gear=GEAR_RV;
							}else
							if ( inputTemp>>GEAR1LO && inputTemp<<GEAR1HI){
									tUserCtrl.Gear=GEAR_1;
							}else
							if ( inputTemp>>GEAR2LO && inputTemp<<GEAR2HI){
									tUserCtrl.Gear=GEAR_2;
							}else
							if ( inputTemp>>GEAR3LO && inputTemp<<GEAR3HI){
									tUserCtrl.Gear=GEAR_3;
							}else
							tUserCtrl.Gear=GEAR_NULL;
					}
			}
}


/*********************************************
Function: Update_InputScan;
DESC: Update input status;
Input: NA;
Output: Update the setting in tUserCtrl;
Return: NA;
**********************************************/
void Update_InputScan(void)
{
			getGearSelected(tADC.Reading_FBVol);
	

}

