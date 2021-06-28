#include "gpio_init.h"
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_Config;
TIM_OCInitTypeDef Timer_init;
TIM_TimeBaseInitTypeDef TimeBase_Init;
void GPIO_Conf(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	
}

void delay_60ns(u32 n)//5.9ns nop
{
	u8 i;
	for(i=0;i<99*n;i++)
	{
		__nop();
	}
}

