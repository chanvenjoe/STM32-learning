#ifndef __HALL_H
#define __HALL_H	 
#include "sys.h"

#define HALL_RCC_CLOCKGPIO      RCC_AHBPeriph_GPIOA

#define HALL_PORT_U             GPIOA
#define HALL_PIN_U              GPIO_Pin_0
#define HALL_PINSOURCE_U        GPIO_PinSource0

#define HALL_PORT_V             GPIOA
#define HALL_PIN_V              GPIO_Pin_1
#define HALL_PINSOURCE_V        GPIO_PinSource1

#define HALL_PORT_W             GPIOA
#define HALL_PIN_W              GPIO_Pin_2
#define HALL_PINSOURCE_W        GPIO_PinSource2

#define HALL_STATE_U            GPIO_ReadInputDataBit(HALL_PORT_U,HALL_U_PIN);
#define HALL_STATE_V            GPIO_ReadInputDataBit(HALL_PORT_V,HALL_V_PIN);
#define HALL_STATE_W            GPIO_ReadInputDataBit(HALL_PORT_W,HALL_W_PIN);

#define HALL_STATE              

void Hall_Init(void);
			    
#endif
