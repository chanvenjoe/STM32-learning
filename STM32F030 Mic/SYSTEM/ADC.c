#include "ADC.h"
#include "stm32f0xx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
GPIO_InitTypeDef GPIO_Typedef_Structure;
TIM_TimeBaseInitTypeDef		 Timer_Structure;
TIM_OCInitTypeDef TIM_OC_structure;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void LED_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	//init PA6\7
	GPIO_Typedef_Structure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Typedef_Structure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Typedef_Structure.GPIO_OType= GPIO_OType_PP;
	GPIO_Typedef_Structure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Typedef_Structure.GPIO_Speed= GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_Typedef_Structure);
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);

	GPIO_Typedef_Structure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_Typedef_Structure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void Timer(uint32_t arr, uint16_t psc)
{
	//timer init
	Timer_Structure.TIM_Prescaler = psc;
	Timer_Structure.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_Structure.TIM_Period = arr; //auto reload register
	Timer_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM14, &Timer_Structure);

	TIM_OC_structure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_structure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC_structure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OC_structure.TIM_Pulse =
	TIM_OC1Init(TIM14, &TIM_OC_structure);

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);

	//GPIO init
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIO_Typedef_Structure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Typedef_Structure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_Typedef_Structure.GPIO_OType= GPIO_OType_PP;
//	GPIO_Typedef_Structure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Typedef_Structure.GPIO_Speed= GPIO_Speed_Level_3;
	GPIO_Init(GPIOC, &GPIO_Typedef_Structure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource13, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource14, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource15, GPIO_AF_4);
}
