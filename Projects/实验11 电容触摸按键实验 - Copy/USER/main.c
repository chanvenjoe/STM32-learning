#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tpad.h"

//ALIENTEK ̽����STM32F407������ ʵ��11
//���ݴ�������ʵ�� -�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
	LED_Init();	
	delay_init(168);
	TPAD_Init(8);
	while(1)
	{
		if(TPAD_Scan(0))
		{
			LED1=!LED1;		//LED1ȡ��
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
