#include "gpio_init.h"
#include "stm32f0xx.h"

GPIO_InitTypeDef GPIO_Config;
TIM_OCInitTypeDef Timer_init;
TIM_TimeBaseInitTypeDef TimeBase_Initt;
void GPIO_Conf(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_Level_3; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_Level_3; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOE, &GPIO_Config);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_Level_3; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOC, &GPIO_Config);
	
}


