#ifndef __LED_H
#define __LED_H	 
#include "HAL_conf.h"

#define LED1_RCC_CLOCKGPIO      RCC_AHBPeriph_GPIOD
#define LED1_PORT               GPIOD
#define LED1_PIN                GPIO_Pin_0

#define LED1_ON()               GPIO_ResetBits(LED1_PORT,LED1_PIN)
#define LED1_OFF()              GPIO_SetBits(LED1_PORT,LED1_PIN)
#define LED1_TOGGLE()           (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?(GPIO_ResetBits(LED1_PORT,LED1_PIN)):(GPIO_SetBits(LED1_PORT,LED1_PIN))

void Led_Init(void);

		 				    
#endif
