#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"

#define T0H GPIO_SetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(3);
#define T0L GPIO_ResetBits(GPIOF,GPIO_Pin_10); 		delay_47_6ns(34);
#define T1H GPIO_SetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(34);
#define T1L GPIO_ResetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(3);
#define RES GPIO_ResetBits(GPIOF,GPIO_Pin_10);		delay_47_6ns(1300);
#define TEST_delay delay_47_6ns(1)
#define CODE_0 T0H T0L
#define CODE_1 T1H T1L

int main(void)
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Conf();
	delay_init(168);
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);


	while(1)
	{
		u8 i;
		u8 w=2;
		u32 c = 0x00800000;
		u32 dat = 0x0000f0; 
		RES
		while(w--)
		{
		for(i=0;i<24;i++)
		{
			if(dat&c)
			{
				CODE_1
			}
			else CODE_0
			dat<<=1;
		}
		}
		//RES
		
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
 



