#include "gpio_init.h"
#include "stm32f4xx.h"
#include "usart.h"	

GPIO_InitTypeDef GPIO_Config;
TIM_OCInitTypeDef Timer_init;
TIM_TimeBaseInitTypeDef TimeBase_Init;
USART_InitTypeDef Usart_init;
NVIC_InitTypeDef NVIC_init;
void GPIO_Conf(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	GPIO_ResetBits(GPIOF, GPIO_Pin_1);
	GPIO_ResetBits(GPIOF, GPIO_Pin_3);
	GPIO_ResetBits(GPIOF, GPIO_Pin_5);
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	GPIO_SetBits(GPIOF, GPIO_Pin_7);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_2MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOE, &GPIO_Config);
}

void Timer_PWM_Init(u32 arr, u16 psc)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_9;
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	
	TimeBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_Init.TIM_Period = arr;
	TimeBase_Init.TIM_Prescaler = psc; //change the TIM clock divition
	TIM_TimeBaseInit(TIM14, &TimeBase_Init);
	TIM_TimeBaseInit(TIM2, &TimeBase_Init);
	
	Timer_init.TIM_OCMode =  TIM_OCMode_PWM1; //PWM1:when up-counting TIMx_CNT<TIMx_CCR1, it is valid PWM2:oppsite
	Timer_init.TIM_OCPolarity = TIM_OCPolarity_High;//  valid value is High
	Timer_init.TIM_OutputState = TIM_OutputState_Enable;
	//Timer_init.TIM_Pulse = 100;
	TIM_OC1Init(TIM14, &Timer_init);
	TIM_OC1Init(TIM2, &Timer_init);
	
	
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void usart_init(u32 baud_rate)
{
	//Open necessary clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP; //the down pulse means star transmition
	GPIO_Init(GPIOF, &GPIO_Config);
	
	Usart_init.USART_BaudRate = baud_rate;
	Usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_init.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	Usart_init.USART_Parity = USART_Parity_No;
	Usart_init.USART_StopBits = USART_StopBits_1;
	Usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &Usart_init);
	USART_Cmd(USART1, ENABLE);
	

#if EN_USART1_RX
	 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_init.NVIC_IRQChannel = USART1_IRQn;//fount from Stm32f4xx.h
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_init);
#endif
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		u8 res = USART_ReceiveData(USART1);
		USART_SendData(USART1, res);
	}
}
