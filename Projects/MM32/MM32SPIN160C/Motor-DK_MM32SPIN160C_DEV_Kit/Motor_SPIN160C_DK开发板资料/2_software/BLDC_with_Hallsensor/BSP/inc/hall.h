#ifndef __HALL_H
#define __HALL_H	 
#include "sys.h"

#define HALL_TIM                TIM2
#define HALL_TIM_CLOCK_CMD      RCC_APB1PeriphClockCmd
#define HALL_TIM_CLK            RCC_APB1Periph_TIM2
#define HALL_TIM_PERIOD         0xFFFF
#define HALL_TIM_PRESCALER      71
#define HALL_TIM_Channel_x      TIM_Channel_1

#define HALL_GPIO_CLK       RCC_AHBPeriph_GPIOA

#define HALL_PORT_U             GPIOA
#define HALL_PIN_U              GPIO_Pin_0
#define HALL_PINSOURCE_U        GPIO_PinSource0

#define HALL_PORT_V             GPIOA
#define HALL_PIN_V              GPIO_Pin_1
#define HALL_PINSOURCE_V        GPIO_PinSource1

#define HALL_PORT_W             GPIOA
#define HALL_PIN_W              GPIO_Pin_2
#define HALL_PINSOURCE_W        GPIO_PinSource2

#define HALL_U_PIN_AF      GPIO_AF_2
#define HALL_V_PIN_AF      GPIO_AF_2
#define HALL_W_PIN_AF      GPIO_AF_2

#define HALL_TIM_IRQn           TIM2_IRQn
#define HALL_TIM_IRQHANDLER     TIM2_IRQHandler

void Hall_Init(void);
			    
#endif
