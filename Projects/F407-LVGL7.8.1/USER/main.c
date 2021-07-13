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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	LCD_Display_Dir(1);
	KEY_Init(); 				//按键初始化  
	TIM3_Int_Init(999,83);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	FSMC_SRAM_Init();		//初始化外部sram
	tp_dev.init();			//触摸屏初始化
	
	lv_init();						//lvgl系统初始化
	lv_port_disp_init();	//lvgl显示接口初始化,放在lv_init()的后面
	lv_port_indev_init();	//lvgl输入接口初始化,放在lv_init()的后面
	
	lv_demo_benchmark();
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
	}
}
