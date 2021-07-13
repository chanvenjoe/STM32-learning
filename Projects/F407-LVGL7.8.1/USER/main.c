#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "timer.h"
#include "sram.h"
#include "touch.h" 
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_examples.h"



int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ�� 
	LCD_Display_Dir(1);
	KEY_Init(); 				//������ʼ��  
	TIM3_Int_Init(999,83);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	FSMC_SRAM_Init();		//��ʼ���ⲿsram
	tp_dev.init();			//��������ʼ��
	
	lv_init();						//lvglϵͳ��ʼ��
	lv_port_disp_init();	//lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ���
	lv_port_indev_init();	//lvgl����ӿڳ�ʼ��,����lv_init()�ĺ���
	
	lv_demo_benchmark();
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
	}
}
