#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
#include "lcd.h"
#include "touch.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_examples.h"

int main(void)
{
	GPIO_Conf();
	delay_init(168);
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	usart_init(115200);
//	External_Interrupt_init();
//	IWDG_Init(8,1000);
//	Timer_PWM_Init(u32 arr, u16 psc);
//	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);
	General_Timer_Interrupt(999,83);//timer3 84M   time=arr*psc/84M = 1ms =>psc=8400 arr = 10
	u32 temp=CapacitiveTouch_Init(8);
	LCD_Init();
//	tp_dev.init();
	lv_init();
	lv_port_disp_init();//example3 is too big, surpass the size
	lv_port_indev_init();
	lv_example_get_started_1();
	while(1)
	{
//		tp_dev.scan(0);
		lv_task_handler();
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
 
