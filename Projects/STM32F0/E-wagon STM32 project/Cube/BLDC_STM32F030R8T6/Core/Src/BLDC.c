/*
 * BLDC.c
 *
 *  Created on: Aug 8, 2023
 *      Author: kzhou
 */

#include "BLDC.h"


void BLDC_Start_Up()
{
	for(unsigned int i = 10000;i>=1000;i-=100)
	{
		delay_us(i);
		AHBL_ON;
		delay_us(i);//delay
		CHBL_ON;
		delay_us(i);
		CHAL_ON;
		delay_us(i);
		BHAL_ON;
		delay_us(i);
		BHCL_ON;
		delay_us(i);
		AHCL_ON;

	}
}
void BLDC_Driving_test()// The driving sequence is 1-5-4-6-2-3
{
	static int i=1;
	switch(i)
	{
	case 1:
		AHBL_ON;
//		printf("AB\r\n");
		break;
	case 6:
		AHCL_ON;
//		printf("CB\r\n");
		break;
	case 5:
		BHCL_ON;
//		printf("CA\r\n");
		break;
	case 4:
		BHAL_ON;
//		printf("BA\r\n");
		break;
	case 3:
		CHAL_ON;
//		printf("BC\r\n");
		break;
	case 2:
		CHBL_ON;
//		printf("AC\r\n");
		break;
	default:
		CLOSE_ALL;
		break;
	}
	i= i==6? 1:i+1;
}

void BLDC_Phase_switching(MADC_Structure * adc_val)
{
		adc_val->commutation_timeout+=1;
		if(adc_val->bemf_now==adc_val->bemf_last&&adc_val->bemf_now!=0)
		{
			adc_val->zero_acrross_flag++;
		}
		else
			adc_val->zero_acrross_flag = 0;
	//	adc_val->commutation_delay = 0;
		if(adc_val->zero_acrross_flag>3)
		{
		switch(adc_val->bemf_now)
		{
			case 5:
				AHBL_ON;
					__HAL_TIM_SET_COUNTER(&htim15, 0);//the auto reload is set to 65535 1us time base
					HAL_TIM_Base_Start(&htim15);
					adc_val->commutation_timeout = 0;
				break;
			case 4:
				AHCL_ON;
				break;
			case 6:
				BHCL_ON;
				break;
			case 2:
				BHAL_ON;
				break;
			case 3:
				CHAL_ON;
				break;
			case 1:
				CHBL_ON;
				adc_val->commutation_delay = __HAL_TIM_GET_COUNTER(&htim15);
				HAL_TIM_Base_Stop(&htim15);
				break;
//			default:
//				BLDC_Driving_test();

		}
		}

}
