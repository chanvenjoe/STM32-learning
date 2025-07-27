#include "FreeRTOS_test_funcs.h"
#include "main.h"
#include "tim.h"
#include "FreeRTOS.h"
void LED_Test(void)
{
	while(1)
	{
		static char state = 0;
		state  = ~state;
		HAL_GPIO_WritePin(LED_IND_GPIO_Port,LED_IND_Pin , state);
		osDelay(1000);
	}
}

void Win1_Test(void)
{

}

void Win2_Test(void)
{

}

void Win3_Test(void)
{

}

void Win4_Test(void)
{
	while(1)
	{
		static char state = 0;
		state  = ~state;
		if(state)
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
		else
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 10);
//		osDelay(3000);
		vTaskDelay(3000);
//		vTaskSuspend(g_TCBofWinTask);
//		vTaskResume(); Recover to the state before suspend
	}
}
