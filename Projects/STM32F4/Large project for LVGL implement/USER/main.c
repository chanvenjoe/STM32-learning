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

	sprintf((char*)lcd_id,"LCD ID:%04x",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"MCU:Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"USMART TEST");	
	LCD_ShowString(30,90,200,16,16,"_____________");
	LCD_ShowString(30,110,200,16,16,"2021/3/20");	   
  	while(1) 
	{		
		GPIOF->BSRRH= GPIO_Pin_9;
		delay_ms(1000);
		GPIOF->BSRRL= GPIO_Pin_9;
		delay_ms(1000);
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
 
