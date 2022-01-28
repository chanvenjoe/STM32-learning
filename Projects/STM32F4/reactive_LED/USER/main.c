#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
#include "WS2811.h"
#include "Speed_display.h"

int main(void)
{
	u32 temp;
	GPIO_Conf();
	delay_init(168);
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Conf();
	delay_init(168);
	temp=CapacitiveTouch_Init(8);
	while(1)
	{
		while(1)
		//while(TPAD_Scan(0,temp))
		{	
//		WS_Key_RGB();
			WS_Hue_change();
		}
	}


}


/**
*******************����ע�͵��Ĵ�����ͨ�� λ�� ����ʵ��IO�ڿ���**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
  while(1)
	{
     LED0=0;			  //LED0��
	   LED1=1;				//LED1��
		 delay_ms(500);
		 LED0=1;				//LED0��
		 LED1=0;				//LED1��
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************����ע�͵��Ĵ�����ͨ�� ֱ�Ӳ����Ĵ��� ��ʽʵ��IO�ڿ���**************************************
int main(void)
{ 
 
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1��
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0��
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1��
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 



