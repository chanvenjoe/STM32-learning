#ifndef _gpio_H
#define _gpio_H	 

#include "common.h"




#define W2_RUN_PORT       GPIOC
#define W2_RUN_PIN        GPIO_Pin_15

#define W1_RUN_PORT      	GPIOC
#define W1_RUN_PIN       	GPIO_Pin_14

#define W3_RUN_PORT       GPIOB
#define W3_RUN_PIN        GPIO_Pin_13

#define W4_RUN_PORT				GPIOD
#define W4_RUN_PIN				GPIO_Pin_2

#define WS2812_PORT				POIOB
#define WS2812_PIN				GPIO_Pin15



typedef enum
{
    WIN_ON,
    WIN_OFF,
}WIN_STATUS_enum;




void w1_run_control(WIN_STATUS_enum status);
void w2_run_control(WIN_STATUS_enum status);
void w3_run_control(WIN_STATUS_enum status);
void w4_run_control(WIN_STATUS_enum status);
void wins_init(void);


#endif



