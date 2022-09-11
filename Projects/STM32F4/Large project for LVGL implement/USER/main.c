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
#include "adc.h"

void led_set(u8 sta)
{
	PFout(10)=sta;
} 
//函数参数调用测试函数

void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  

int main(void)
{
	GPIO_Conf();
	delay_init(168);
	usart_init(115200);
	usmart_dev.init(84); 	//初始化USMART
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	External_Interrupt_init();
//	IWDG_Init(8,1000);
//	Timer_PWM_Init(u32 arr, u16 psc);
//	WWDG_Init(0x7f,0x5f,WWDG_Prescaler_8);
//	General_Timer_Interrupt(999,83);//timer3 84M   time=arr*psc/84M = 1ms =>psc=8400 arr = 10 for LVGL
//	u32 temp=CapacitiveTouch_Init(8);
	LCD_Init();           //初始化LCD FSMC接口
	ADC_Init_Config();
	u8 lcd_id[12];				//存放LCD ID字符串
	while(RNG_Init())
	{
		LCD_ShowString(30,130,200,16,16, "RNG Init failed");
		delay_ms(20);
		LCD_ShowString(30,150,200,16,16, "Rebooting");
	}
	LCD_ShowString(30,130,200,16,16, "RNG Ready");
	LCD_ShowString(30,150,200,16,16, "Press Key0 to get a random number");

	sprintf((char*)lcd_id,"LCD ID:%04x",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
	POINT_COLOR=BLACK; 
	u8 range;
  	while(1) 
	{	
		float temp;
		int adc = Get_ADC_Average(ADC1,ADC_Channel_5,20);
		LCD_ShowString(10,230,200,16,16,"ADC value:");
		LCD_ShowxNum(134,230,adc,4,16,0);
		//PA5 Voltage
		temp = (float) adc*(3.3/4096);
		adc = temp;//temp tens to ADC
		LCD_ShowString(10,250,200,16,16,"ADC vol:");
		LCD_ShowxNum(118,250,adc,4,16,0);//tens part
//		LCD_ShowString(150,250,200,16,16,".");
		adc = (temp-adc)*1000;
		LCD_ShowxNum(150,250,adc,4,16,0);
		LCD_ShowString(198,250,200,16,16,"V");
		//Temperature sensor internal
		adc = Get_ADC_Average(ADC1,ADC_Channel_TempSensor,20);
		temp = (float) adc*(3.3/4096);
		temp = (temp-0.76)/0.0025+25;
		adc = temp;
		LCD_ShowString(10,266,200,16,16,"Temperature:");
		LCD_ShowxNum(216,266,adc,4,16,0);
		LCD_ShowString(264,266,200,16,16,"degree");
		//Light sensor
		adc = Get_ADC_Average(ADC3, ADC_Channel_5,20);
		LCD_ShowString(10,282,200,16,16,"Brightness:");
		LCD_ShowxNum(216,282,adc,4,16,0);
		
		u8 key;
		key = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);
		if(key==0);
		{
			LCD_ShowNum(30+8*16,300,RNG_GetRandomNumber(),10,16);
		}
		range = RNG_Get_RandomRange(20,30);
		LCD_ShowNum(30+8*16,320,range ,1,16);
		delay_ms(300);
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
 
