#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
//#include "LCD_init.h"
#include "lcd.h"
#include "touch.h"
//#include "lv_port_disp.h"
//#include "lv_port_indev.h"
#include "usmart.h"
#include "rng.h"

void led_set(u8 sta)
{
	PFout(10)=sta;
} 
//�����������ò��Ժ���

void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  

int main(void)
{
	GPIO_Conf();
	delay_init(168);
	usart_init(115200);
	usmart_dev.init(84); 	//��ʼ��USMART
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	External_Interrupt_init();
//	IWDG_Init(8,1000);
//	Timer_PWM_Init(u32 arr, u16 psc);
//	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);
//	General_Timer_Interrupt(999,83);//timer3 84M   time=arr*psc/84M = 1ms =>psc=8400 arr = 10 for LVGL
//	u32 temp=CapacitiveTouch_Init(8);
	LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	u8 lcd_id[12];				//���LCD ID�ַ���
	while(RNG_Init())
	{
		LCD_ShowString(30,130,200,16,16, "RNG Init failed");
		delay_ms(20);
		LCD_ShowString(30,150,200,16,16, "Rebooting");
	}
	LCD_ShowString(30,130,200,16,16, "RNG Ready");
	LCD_ShowString(30,150,200,16,16, "Press Key0 to get a random number");

	sprintf((char*)lcd_id,"LCD ID:%04x",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	
	POINT_COLOR=BLACK; 
	u8 range;
  	while(1) 
	{	
		u8 key;
		key = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);
		if(key==0);
		{
			LCD_ShowNum(30+8*16,210,RNG_GetRandomNumber(),10,16);
		}
		range = RNG_Get_RandomRange(20,30);
		LCD_ShowNum(30+8*16,320,range ,1,16);
		delay_ms(300);
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
 
