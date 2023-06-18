
#include "HAL_conf.h"
#include "gpio.h"


#define LED_RUN_PORT        GPIOC
#define LED_RUN_PIN         GPIO_Pin_15

#define LED_FAULT_PORT      GPIOC
#define LED_FAULT_PIN       GPIO_Pin_14

#define LED_EN_PORT         GPIOB
#define LED_EN_PIN          GPIO_Pin_13



void led_run_control(LED_STATUS_enum status)
{
    GPIO_WriteBit(LED_RUN_PORT, LED_RUN_PIN, (BitAction)status);
}


void led_fault_control(LED_STATUS_enum status)
{
    GPIO_WriteBit(LED_FAULT_PORT, LED_FAULT_PIN, (BitAction)status);
}

void led_en_control(LED_STATUS_enum status)
{
    GPIO_WriteBit(LED_EN_PORT, LED_EN_PIN, (BitAction)status);
}

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE); 

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    
	GPIO_SetBits(GPIOC, GPIO_Pin_14);
    GPIO_SetBits(GPIOC, GPIO_Pin_15);
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);

    //C12引脚必须设置为输出高电平
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_12);                        
}




