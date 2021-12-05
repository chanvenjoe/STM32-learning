#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
#include "WS2811.h"
#include "Speed_display.h"

int main(void)
{
	GPIO_Conf();
	delay_init(168);
	GPIO_ResetBits(GPIOC,0xff);
//	GPIO_SetBits(GPIOF,0xff);

	while(1)
	{
		u8 c;
		for(c=10;c<255;c++)
		{
			numb_show(c);
		}
	}
	
//	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	u32 temp;
//	GPIO_Conf();
//	delay_init(168);
//	temp=CapacitiveTouch_Init(8);
//	while(1)
//	{
//		while(TPAD_Scan(0,temp))
//		{	
//			WS_Key_RGB();
//			GPIO_ResetBits(GPIOF,GPIO_Pin_5);
//			delay_ms(100);
//			GPIO_SetBits(GPIOF,GPIO_Pin_5);
//		}
//	}


}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



