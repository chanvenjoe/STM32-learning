/*
 * BLDC.c
 *
 *  Created on: Aug 8, 2023
 *      Author: kzhou
 */

#include "BLDC.h"


void BLDC_Start_Up()
{
	unsigned int j=0,i=0;
	for( i = 0;i<273;i++)
	{
		j = 30000-i*100;
		delay_us(j);
		AHBL_ON;
		delay_us(j);//delay
		CHBL_ON;
		delay_us(j);
		CHAL_ON;
		delay_us(j);
		BHAL_ON;
		delay_us(j);
		BHCL_ON;
		delay_us(j);
		AHCL_ON;
	}
	for(i=0;i<800;i++)
	{
		j = 2700;
		delay_us(j);
		AHBL_ON;
		delay_us(j);//delay
		CHBL_ON;
		delay_us(j);
		CHAL_ON;
		delay_us(j);
		BHAL_ON;
		delay_us(j);
		BHCL_ON;
		delay_us(j);
		AHCL_ON;
	}
}
void BLDC_Driving_test(MADC_Structure * adc_val)// The driving sequence is 1-5-4-6-2-3 CBA
{
	static int i=1;
	switch(i)
	{
	case 1:
		AHBL_ON;
		__HAL_TIM_SET_COUNTER(&htim15, 0);//the auto reload is set to 65535 1us time base
		HAL_TIM_Base_Start(&htim15);
//		printf("AB\r\n");
		break;
	case 2:
		AHCL_ON;
//		printf("CB\r\n");
		break;
	case 3:
		BHCL_ON;
//		printf("CA\r\n");
		break;
	case 4:
		BHAL_ON;
//		printf("BA\r\n");
		break;
	case 5:
		CHAL_ON;
//		printf("BC\r\n");
		break;
	case 6:
		CHBL_ON;
		adc_val->speed = __HAL_TIM_GET_COUNTER(&htim15);//1us base  //but only count 5 times
		HAL_TIM_Base_Stop(&htim15);
//		printf("AC\r\n");
		break;
	default:
//		CLOSE_ALL;
		break;
	}
	i= i==6? 1:i+1;

}

void BLDC_Phase_switching(MADC_Structure * adc_val)
{
		static uint16_t delay;
		delay = delay<=100? adc_val->commutation_delay: delay-100;
		adc_val->commutation_timeout+=1;
		if(delay<=100)
		{

			switch(adc_val->bemf_next)
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
					adc_val->speed = __HAL_TIM_GET_COUNTER(&htim15);
					HAL_TIM_Base_Stop(&htim15);
					break;
			}
		}

}

void BLDC_PWM_Handle(uint8_t duty)
{
	duty = duty>=0&&duty<=50? duty:0;//Value range confirm
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty);
}

