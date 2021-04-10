#include "ADC.h"
#include "stm32f0xx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
GPIO_InitTypeDef GPIO_Typedef_Structure;
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
