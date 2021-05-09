#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"

int main(void)
{
	u16 pwm = 0;
	u8 dir = 1;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	GPIO_Conf();
	delay_init(168);
	Timer_PWM_Init(500-1,84-1); //contains timer2 initialization
	while(1)
	{
		delay_ms(5);
		if(dir)pwm++;
		else pwm--;
		if(pwm>500)dir=0;
		if(pwm==0)dir=1;
		
		TIM_SetCompare1(TIM14, pwm);// high value percentage
		TIM_SetCompare1(TIM2, pwm);
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
 



