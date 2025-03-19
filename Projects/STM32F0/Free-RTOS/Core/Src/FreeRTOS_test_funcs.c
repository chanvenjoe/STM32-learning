#include "FreeRTOS_test_funcs.h"
#include "main.h"

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
