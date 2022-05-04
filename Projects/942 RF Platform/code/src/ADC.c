#include "ADC.h"




/*********************************************
Function: get_ADC_Value;
DESC: get ADC value from specific channel;
Input: channel;
Output: NA;
Return: ADC reading from specific;
**********************************************/
static unsigned int get_ADC_Value(unsigned int Channel){
		
			if(Channel==0)
			{
					UINT16 sumThrottle=0;
					UINT8 i;
					Enable_ADC_AIN0;    //P17 throttle AIN0
					clr_ADCF;						//clear the ADC flag 
					set_ADCS; 					//start the ADC reading
					EADC=0;  						//disable the interruption
					
					for(i=0; i<8; i++){
								sumThrottle+=((ADCRH<<4)+ADCRL);
					}
					Disable_ADC;
					return sumThrottle>>3;
			}else
			if(Channel==1)
			{
					UINT16 sumFB=0;
					UINT8 i;
					Enable_ADC_AIN1;    //P17 throttle AIN1
					clr_ADCF;						//clear the ADC flag 
					set_ADCS; 					//start the ADC reading
					EADC=0;  						//disable the interruption
					
					for(i=0; i<8; i++){
								sumFB+=((ADCRH<<4)+ADCRL);
					}
					Disable_ADC;
					return sumFB>>3;
			}else
			if(Channel==4)
			{	
					UINT16 sumCurrent=0;
					UINT8 i;
					Enable_ADC_AIN4;    //P17 throttle AIN0
					clr_ADCF;						//clear the ADC flag 
					set_ADCS; 					//start the ADC reading
					EADC=0;  						//disable the interruption
					
					for(i=0; i<8; i++){
								sumCurrent+=((ADCRH<<4)+ADCRL);
					}
					Disable_ADC;
					return sumCurrent>>3;
			}else
			if(Channel==7)
			{
					UINT16 sumVoltage=0;
					UINT8 i;
					Enable_ADC_AIN7;    //P17 throttle AIN0
					clr_ADCF;						//clear the ADC flag 
					set_ADCS; 					//start the ADC reading
					EADC=0;  						//disable the interruption
					
					for(i=0; i<8; i++){
								sumVoltage+=((ADCRH<<4)+ADCRL);
					}
					Disable_ADC;
					return sumVoltage>>3;
			}else
			
			return 0;

}



/*********************************************
Function: ADC_Refresh;
DESC: Refresh the ADC voltage for gear, throttle, current and bus voltage;
Input: NA;
Output: Update the ADC value in tADC;
Return: NA;
**********************************************/
void ADC_Refresh(void)
{
		UINT16 Temp;
		Temp = tSysClock.ADCCount;
		
		if (Temp!= tADC.Count)
		{
			tADC.Count = Temp;
			switch(Temp)
			{
				case 0:tADC.Reading_ThrottleVol=get_ADC_Value(ChannelThrottle);
						break;
				case 2:tADC.Reading_FBVol=get_ADC_Value(ChannelFB);
						break;
				case 4:tADC.Reading_Current=get_ADC_Value(ChannelCurrent);
						break;
				case 6:tADC.Reading_Voltage=get_ADC_Value(ChannelVoltage);
						break;
			}
		}
}


		
