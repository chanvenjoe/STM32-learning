#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
  	 
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}key_state_t;

#define BKEY_GPIO_CLK           (RCC_AHBPeriph_GPIOB)
#define KEY1_PIN                GPIO_Pin_8
#define KEY1_PORT               GPIOB
#define KEY1_DOWN_LEVEL         1  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ1 */
#define KEY2_PIN                GPIO_Pin_9
#define KEY2_PORT               GPIOB
#define KEY2_DOWN_LEVEL         1  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ0 */

extern void key_init(void);
extern key_state_t key1_scan(void);
extern key_state_t key2_scan(void);
				    
#endif
