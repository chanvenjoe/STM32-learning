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
#include "dac.h"
#include "dma.h"

void ADC_DAC_Display(void);
	
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
	MDAC_Init();
	TIM9_PWM_Init(255,0);//PWM DAC
	MyDMA_Config(DMA2_Stream7,DMA_Channel_4, (u32)&USART1->DR
	u8 lcd_id[12];				//存放LCD ID字符串
	while(RNG_Init())
	{
		LCD_ShowString(Text_x,Text_Top_y,200,16,16, "RNG Init failed");
		delay_ms(20);
		LCD_ShowString(Text_x,Text_Top_y*1,200,16,16, "Rebooting");
	}
	LCD_ShowString(Text_x,Text_Top_y*0,200,16,16, "RNG Ready");
	LCD_ShowString(Text_x,Text_Top_y*1,200,16,16, "Press Key0 to get a random number");

	sprintf((char*)lcd_id,"LCD ID:%04x",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
	POINT_COLOR=BLACK; 
  	while(1) 
	{	
		ADC_DAC_Display();
		
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
void ADC_DAC_Display()
{
	u8 range;
	double temp;
	static u16 adcx,pwmval=0;
	int adc = Get_ADC_Average(ADC1,ADC_Channel_5,20);
	LCD_ShowString(Text_x,Text_Top_y*3,200,16,16,"ADC value:");
	LCD_ShowxNum(134,Text_Top_y*3,adc,4,16,0);
	//PA5 Voltage
	temp = (float) adc*(3.3/4096);
	adc = temp;//temp tens to ADC
	LCD_ShowString(Text_x,Text_Top_y*4,200,16,16,"ADC vol:");
	LCD_ShowxNum(118,Text_Top_y*4,adc,4,16,0);//tens part
//		LCD_ShowString(150,250,200,16,16,".");
	adc = (temp-adc)*1000;
	LCD_ShowxNum(150,Text_Top_y*4,adc,4,16,0);
	LCD_ShowString(198,Text_Top_y*4,200,16,16,"V");
	//Temperature sensor internal
	adc = Get_ADC_Average(ADC1,ADC_Channel_TempSensor,20);
	temp = (float) adc*(3.3/4096);
	temp = (temp-0.76)/0.0025+25;
	adc = temp;
	LCD_ShowString(10,Text_Top_y*6,200,16,16,"Temperature:");
	LCD_ShowxNum(216,Text_Top_y*6,adc,4,16,0);
	LCD_ShowString(264,Text_Top_y*6,200,16,16,"degree");
	//Light sensor
	adc = Get_ADC_Average(ADC3, ADC_Channel_5,20);
	LCD_ShowString(10,Text_Top_y*7,200,16,16,"Brightness:");
	LCD_ShowxNum(216,Text_Top_y*7,adc,4,16,0);
	//DAC
	
	u8 key;
	static u16 dac_val=0;
	key = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);
	if(key==0X00)
	{
		pwmval+=10;
		dac_val+=10;
		TIM_SetCompare2(TIM9, pwmval);
		adcx = TIM_GetCapture2(TIM9);
		temp = (float)adcx*(3.3/256);//PWM DAC voltage
		adcx = temp;
		LCD_ShowString(10,Text_Top_y*11,200,16,16,"PWM DAC:");
		LCD_ShowNum(30+8*16,Text_Top_y*11,pwmval,10,16);
		temp = (temp-adcx)*1000;
		LCD_ShowString(10,Text_Top_y*12,200,16,16,"PWM DAC voltage:");
		LCD_ShowNum(30+6*16,Text_Top_y*12,adcx,3,16);
		LCD_ShowNum(30+10*16,Text_Top_y*12,temp,11,16);



		if(pwmval>=250)
		{
			pwmval = 0;
		}
		
		LCD_ShowString(10,Text_Top_y*9,200,16,16,"RNG number:");
		LCD_ShowNum(30+8*16,Text_Top_y*9,RNG_GetRandomNumber(),10,16);	
		DAC_SetChannel1Data(DAC_Align_12b_R,dac_val);	
	}
	range = RNG_Get_RandomRange(20,30);
	LCD_ShowString(10,Text_Top_y*10,200,16,16,"RNG Range:");
	LCD_ShowNum(30+8*16,Text_Top_y*10,range ,1,16);
	
	LCD_ShowString(10,Text_Top_y*8,200,16,16,"DAC_VALUE:");
	LCD_ShowxNum(216,Text_Top_y*8,dac_val,4,16,0);
	delay_ms(300);
}
