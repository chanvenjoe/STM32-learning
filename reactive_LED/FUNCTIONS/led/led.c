#include "led.h"
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_Config;

void LED_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_2MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
}
