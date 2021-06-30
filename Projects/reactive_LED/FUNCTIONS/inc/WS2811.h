#ifndef __WS2811_H
#define __WS2811_H

#define T0H GPIO_SetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(3);
#define T0L GPIO_ResetBits(GPIOF,GPIO_Pin_10); 		delay_47_6ns(34);
#define T1H GPIO_SetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(34);
#define T1L GPIO_ResetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(3);
#define RES GPIO_ResetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(1300);
#define TEST_delay delay_47_6ns(1)
#define CODE_0 T0H T0L
#define CODE_1 T1H T1L
#define LEDNUM 			2

#endif
