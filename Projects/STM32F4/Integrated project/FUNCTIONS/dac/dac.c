#include "dac.h"

GPIO_InitTypeDef 	DAC_IOcfg;
DAC_InitTypeDef		DAC_Config;
//DAC PC4
void MDAC_Init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC_IOcfg.GPIO_Mode 	= GPIO_Mode_AN;
	DAC_IOcfg.GPIO_Pin		= GPIO_Pin_4;
	DAC_IOcfg.GPIO_PuPd		= GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &DAC_IOcfg);

	DAC_Config.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_Config.DAC_OutputBuffer					= DAC_OutputBuffer_Disable;
	DAC_Config.DAC_Trigger						= DAC_Trigger_None;
	DAC_Config.DAC_WaveGeneration				= DAC_WaveGeneration_None;
	DAC_Init(DAC_Channel_1,&DAC_Config);		
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

void DAC1_Set_Vol(u16 vol)
{
	double temp = vol;
	temp = temp/1000;
	temp = temp*4095/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);
}
//PWM ADC
void TIM9_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef 		PWM_DAC_T9IO;
	TIM_TimeBaseInitTypeDef TIM9_Structure;
	TIM_OCInitTypeDef		TIM_OCInitstructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	PWM_DAC_T9IO.GPIO_Mode 		= GPIO_Mode_AF;
	PWM_DAC_T9IO.GPIO_Pin		= GPIO_Pin_3;
	PWM_DAC_T9IO.GPIO_PuPd		= GPIO_PuPd_UP;
	PWM_DAC_T9IO.GPIO_OType		= GPIO_OType_PP;
	PWM_DAC_T9IO.GPIO_Speed		= GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA, &PWM_DAC_T9IO);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9);
	
	TIM9_Structure.TIM_ClockDivision		= TIM_CKD_DIV1;//NO DIVISION
	TIM9_Structure.TIM_CounterMode			= TIM_CounterMode_Up;
	TIM9_Structure.TIM_Period				= arr;
	TIM9_Structure.TIM_Prescaler			= psc;
	TIM_TimeBaseInit(TIM9, &TIM9_Structure);
	
	TIM_OCInitstructure.TIM_OCIdleState 	= 0;
	TIM_OCInitstructure.TIM_OCMode			= TIM_OCMode_PWM1;
//	TIM_OCInitstructure.TIM_OCNIdleState	= 0;
//	TIM_OCInitstructure.TIM_OCNPolarity		= TIM_OCPolarity_High;
	TIM_OCInitstructure.TIM_OCPolarity		= 0;
//	TIM_OCInitstructure.TIM_OutputNState	= TIM_OutputNState_Enable;
	TIM_OCInitstructure.TIM_OutputState		= TIM_OutputState_Enable;
	TIM_OC2Init(TIM9, &TIM_OCInitstructure);
	
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM9,ENABLE);
	TIM_Cmd(TIM9,ENABLE);
}

