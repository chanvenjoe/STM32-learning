
#include "HAL_conf.h"
#include "gpio.h"


#define W2_RUN_PORT       GPIOC
#define W2_RUN_PIN        GPIO_Pin_15

#define W1_RUN_PORT      	GPIOC
#define W1_RUN_PIN       	GPIO_Pin_14

#define W3_RUN_PORT       GPIOB
#define W3_RUN_PIN        GPIO_Pin_13

#define W4_RUN_PORT				GPIOD
#define W4_RUN_PIN				GPIO_Pin_2

//PB15 needs isolation resistor 
//Motor need diode to keep current flow
void w1_run_control(WIN_STATUS_enum status)
{
    GPIO_WriteBit(W1_RUN_PORT, W1_RUN_PIN, (BitAction)status);
}


void w2_run_control(WIN_STATUS_enum status)
{
    GPIO_WriteBit(W2_RUN_PORT, W2_RUN_PIN, (BitAction)status);
}

void w3_run_control(WIN_STATUS_enum status)
{
    GPIO_WriteBit(W3_RUN_PORT, W3_RUN_PIN, (BitAction)status);
}

void w4_run_control(WIN_STATUS_enum status)
{
    GPIO_WriteBit(W4_RUN_PORT, W4_RUN_PIN, (BitAction)status);
}

void wins_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE); 

		GPIO_InitStructure.GPIO_Pin  =  W1_RUN_PIN|W2_RUN_PIN;//W1 W2 share the same port
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(W1_RUN_PORT, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin  =  W3_RUN_PIN;
		GPIO_Init(W3_RUN_PORT, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin  =  W4_RUN_PIN;
		GPIO_Init(W4_RUN_PORT, &GPIO_InitStructure);

			
		GPIO_SetBits(W1_RUN_PORT, W1_RUN_PIN);
		GPIO_SetBits(W2_RUN_PORT, W2_RUN_PIN);
		GPIO_SetBits(W3_RUN_PORT, W3_RUN_PIN);
		GPIO_SetBits(W4_RUN_PORT, W4_RUN_PIN);

    //C12引脚必须设置为输出高电平  MOTOR DERIVER ENABLE PORT, SET HIGH EN ENABLE
		GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC, GPIO_Pin_12);                        
}




