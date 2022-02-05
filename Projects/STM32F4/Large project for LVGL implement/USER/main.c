#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
//#include "LCD_init.h"
#include "lcd.h"
#include "touch.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

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
//	General_Timer_Interrupt(999,83);//timer3 84M   time=arr*psc/84M = 1ms =>psc=8400 arr = 10 for LVGL
//	u32 temp=CapacitiveTouch_Init(8);
//	LCD_Init();

	u8 lcd_id[12];				//存放LCD ID字符串
		LCD_Init();           //初始化LCD FSMC接口
	POINT_COLOR=RED;      //画笔颜色：红色
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
  	while(1) 
	{		 
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2014/5/4");	      					 
	}
}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 
