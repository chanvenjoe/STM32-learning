#include "FreeRTOS_test_funcs.h"
#include "main.h"
#include "tim.h"
#include "FreeRTOS.h"
#include "WS2811.h"
#include "cmsis_os.h"
void LED_Test(void)
{
	while(1)
	{
		static char state = 0;
		state  = ~state;
//		HAL_GPIO_WritePin(LED_IND_GPIO_Port,LED_IND_Pin , state);
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

void WS2812_LEDStateUpdate(void)
{
	char led_sta = 0;
	for(;;)
	{
		led_sta = WS_LED_State_Get();
		switch(led_sta)
		{
			case 1:////Bat on 0001 0011
			{
				WS_Hue_change();
				break;
			}
			case 2: //USB DETECTED 0010
			case 3:
			{
				WS_ColorSet_LED(0, LEDNUM, 0xff0000);//Solid Green  When USB plug in, the bat will be high
				WS_Refresh();
				break;
			}
			case 7: //CHARGING 0111
			{
				WS_Charging_LED();
				break;
			}
			default://Outside power
			{
				WS_ColorSet_LED(0, LEDNUM, Blue);//Breathing RGB
				WS_Refresh();
				break;
			}
		}
		vTaskDelay(1);
		//	  HAL_GPIO_WritePin(WS_LED_PORT,WS_LED_Pin , GPIO_PIN_SET);//this command takes 180ms, try register

		/*HAL_GPIO_WritePin(WS_LED_PORT,WS_LED_Pin , GPIO_PIN_SET);
		(&htim14)->Instance->CNT = 0;
		while ((&htim14)->Instance->CNT < 2);
		HAL_GPIO_WritePin(WS_LED_PORT,WS_LED_Pin , GPIO_PIN_RESET);*/
	}
}
