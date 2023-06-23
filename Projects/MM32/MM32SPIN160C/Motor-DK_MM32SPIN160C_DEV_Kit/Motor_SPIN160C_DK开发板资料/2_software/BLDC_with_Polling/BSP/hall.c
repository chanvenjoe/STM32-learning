#include "hall.h"

void Hall_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(HALL_RCC_CLOCKGPIO,ENABLE);

    GPIO_InitStructure.GPIO_Pin = HALL_PIN_U;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(HALL_PORT_U, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = HALL_PIN_V;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(HALL_PORT_V, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = HALL_PIN_W;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(HALL_PORT_W, &GPIO_InitStructure);
}

