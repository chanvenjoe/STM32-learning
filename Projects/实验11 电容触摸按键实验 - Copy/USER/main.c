#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tpad.h"

//ALIENTEK 探索者STM32F407开发板 实验11
//电容触摸按键实验 -库函数版本 
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

int main(void)
{
	LED_Init();	
	delay_init(168);
	TPAD_Init(8);
	while(1)
	{
		if(TPAD_Scan(0))
		{
			LED1=!LED1;		//LED1取反
			BEEP=!BEEP;
			PF7=!PF7;
			delay_ms(300);
			LED1=!LED1;
			BEEP=!BEEP;
			PF7=!PF7;
		}
		LED0=~LED0;
		delay_ms(100);
	}
}
