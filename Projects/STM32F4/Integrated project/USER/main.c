#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "gpio_init.h"
//#include "LCD_init.h"
#include "lcd.h"
//#include "touch.h"
//#include "lv_port_disp.h"
//#include "lv_port_indev.h"
#include "key.h"
#include "usmart.h"
#include "rng.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tcs34725.h"
#include "ws2811.h"
#include "w25qxx.h" 
#include "led.h"
#include "flash.h"
#include "sram.h"
#include "malloc.h"

/***************Define**************/
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
const u8 Flash_Text[] ={"STM32 FLASH TEST"};


#define TEXT_LENGTH sizeof(Flash_Text)
#define Wordlen TEXT_LENGTH/4+((TEXT_LENGTH%4)?1:0)
#define FLASH_SAVE_ADDR 0x08010000
#define SEND_BUF_SIZE 80	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
#define SIZE sizeof(TEXT_Buffer)	

u32 FLASH_SIZE = 16*1024*1024;	//FLASH 大小为16字节
//u32 testsram[250000] __attribute__((at(0x68000000))); //250000*32/8 <1MB
/******Variables and constants******/
u32 color_hex;
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区
const u8 TEXT_TO_SEND[]={"DMA->UART Experiment"};
u8 datatemp[SIZE];
u8 Flashdata[TEXT_LENGTH];


/**************Structure************/
COLOR_RGBC rgb;
COLOR_HSL  hsl;



/*********Function declair***********/
void ADC_DAC_Display(void);
void LCD_Pre_display(void);

void led_set(u8 sta)
{
	PFout(10)=sta;
} 
//函数参数调用测试函数

void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  
/***********************************/

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
	MyDMA_Config(DMA2_Stream7,DMA_Channel_4, (u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
//	MyI2C_Init();
	W25QXX_Init();
	LCD_Pre_display();
	FSMC_SRAM_Init();
	fsmc_sram_test(Text_x,Text_Top_y*FSMC_SRAM);

  while(1) 
	{
		ADC_DAC_Display();
	}
}


void ADC_DAC_Display()
{
	
	static u16 adcx,pwmval=0;
	u8 range;
	double temp;
	u8 key;
	u8 datatemp[SIZE];
	float pro=0;//proportion
	static u16 dac_val=0;
	int adc = Get_ADC_Average(ADC1,ADC_Channel_5,20);
	LCD_ShowString(Text_x,Text_Top_y*ADC_VAVLUE,200,16,16,"ADC value:");
	LCD_ShowxNum(134,Text_Top_y*ADC_VAVLUE,adc,4,16,0);
	//PA5 Voltage
	temp = (float) adc*(3.3/4096);
	adc = temp;//temp tens to ADC
	LCD_ShowString(Text_x,Text_Top_y*ADC_VOL,200,16,16,"ADC vol:");
	LCD_ShowxNum(118,Text_Top_y*ADC_VOL,adc,4,16,0);//tens part
//		LCD_ShowString(150,250,200,16,16,".");
	adc = (temp-adc)*1000;
	LCD_ShowxNum(150,Text_Top_y*ADC_VOL,adc,4,16,0);
	LCD_ShowString(198,Text_Top_y*ADC_VOL,200,16,16,"V");
	//Temperature sensor internal
	adc = Get_ADC_Average(ADC1,ADC_Channel_TempSensor,20);
	temp = (float) adc*(3.3/4096);
	temp = (temp-0.76)/0.0025+25;
	adc = temp;
	LCD_ShowString(Text_x,Text_Top_y*TEMPERATURE,200,16,16,"Temperature:");
	LCD_ShowxNum(216,Text_Top_y*TEMPERATURE,adc,4,16,0);
	LCD_ShowString(264,Text_Top_y*TEMPERATURE,200,16,16,"degree");
	//Light sensor
	adc = Get_ADC_Average(ADC3, ADC_Channel_5,20);
	LCD_ShowString(Text_x,Text_Top_y*BRIGHTNESS,200,16,16,"Brightness:");
	LCD_ShowxNum(216,Text_Top_y*BRIGHTNESS,adc,4,16,0);
	//DAC
	
	key = KEY_Scan(0);
	
	if(key==0X00)
	{
		pwmval+=10;
		dac_val+=10;
		TIM_SetCompare2(TIM9, pwmval);
		adcx = TIM_GetCapture2(TIM9);
		temp = (float)adcx*(3.3/256);//PWM DAC voltage
		adcx = temp;
		LCD_ShowString(Text_x,Text_Top_y*PWM_DAC,8*8,16,16,"PWM DAC:");
		LCD_ShowNum(Text_x+8*8,Text_Top_y*PWM_DAC,pwmval,10,16);
		temp = (temp-adcx)*1000;
		LCD_ShowString(Text_x,Text_Top_y*PWM_DAC_VOL,8*16,16,16,"PWM DAC voltage:");
		LCD_ShowNum(Text_x+8*17,Text_Top_y*PWM_DAC_VOL,adcx,1,16);//The 1 means length and it start from right
		LCD_ShowNum(Text_x+8*18,Text_Top_y*PWM_DAC_VOL,temp,4,16);// Since the little number are a seperate so the "0" will no be displayed
		
		printf("\r\nDMA DATA:\r\n"); 	    
		LCD_ShowString(Text_x,Text_Top_y*DMA_TRANSMIT_STATUS,200,16,16,"Start Transimit....");
		LCD_ShowString(30,Text_Top_y*DMA_PERCENTAGE,200,16,16,"   %") ;     //显示百分号    
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
		My_DMA_Enable(DMA2_Stream7,SEND_BUF_SIZE);     //开始一次DMA传输！	  
		    //等待DMA传输完成，此时我们来做另外一些事，点灯
		    //实际应用中，传输数据期间，可以执行另外的任务
		while(1)
		{
			if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
			{
				DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志
				break; 
			}
			pro=DMA_GetCurrDataCounter(DMA2_Stream7);//得到当前还剩余多少个数据
			pro=1-pro/SEND_BUF_SIZE;//得到百分比	  
			pro*=100;      			    //扩大100倍
			LCD_ShowNum(10,Text_Top_y*DMA_PERCENTAGE,pro,3,16);	  
		}		    
		LCD_ShowNum(10,Text_Top_y*DMA_PERCENTAGE,100,3,16);//显示100%	  
		LCD_ShowString(30,Text_Top_y*DMA_TRANSMIT_STATUS,200,16,16,"Transimit Finished!");//提示传送完成
		
				/*******Color sensor*********/
//		TCS34725_GetRawData(&rgb);
//		RGBtoHSL(&rgb,&hsl);	
//		printf("R=%d G=%d B=%d C=%d\r\n",rgb.r,rgb.g,rgb.b,rgb.c);
//		printf("H=%d S=%d L=%d\r\n",hsl.h,hsl.s,hsl.l);
//		color_hex = 0x00;
//		color_hex |= rgb.r; color_hex<<=8;
//		color_hex |= rgb.g; color_hex<<=8;
//		color_hex |= rgb.b;
//		LCD_ShowString(10,Text_Top_y*15,200,16,16,"The color RGB value is:");
//		LCD_ShowNum(30+10*16,Text_Top_y*15,color_hex,11,16);
//		WS_ColorSet_LED(0,20, color_hex);
//		WS_Refresh();
		
		LCD_ShowString(Text_x,Text_Top_y*W25Q128_STATUS,200,16,16,"Start Writing W25Q128....");
		W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
		LCD_ShowString(Text_x,Text_Top_y*W25Q128_STATUS,200,16,16,"W25Q128 Write Finished!");	//提示传送完成

		LCD_ShowString(Text_x,Text_Top_y*W25Q128_READ,200,16,16,"Read W25Q128:");
		W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
		LCD_ShowString(Text_x+8*13,Text_Top_y*W25Q128_READ,200,16,16,datatemp);					//显示读到的字符串


		if(pwmval>=250||dac_val>=250)
		{
			pwmval = 0;
			dac_val = 0;
		}
		
		LCD_ShowString(Text_x,Text_Top_y*RNG_NUM,200,16,16,"RNG number:");
		LCD_ShowNum(30+8*16,Text_Top_y*RNG_NUM,RNG_GetRandomNumber(),10,16);	
		DAC_SetChannel1Data(DAC_Align_12b_R,dac_val);	
	}
	else if(key == 1)
	{
	//	mymalloc(SRAMIN,1024);
		
	}
	range = RNG_Get_RandomRange(20,30);
	LCD_ShowString(Text_x,Text_Top_y*RNG_RANGE,200,16,16,"RNG Range:");
	LCD_ShowNum(30+8*16,Text_Top_y*RNG_RANGE,range ,1,16);
	
	LCD_ShowString(Text_x,Text_Top_y*DAC_VALUE,200,16,16,"DAC_VALUE:");
	LCD_ShowxNum(216,Text_Top_y*DAC_VALUE,dac_val,4,16,0);
	
	
}

void LCD_Pre_display()
{
	u16 i;
	u8 j,mask,t;
	u8 lcd_id[12];				//存放LCD ID字符串
	while(RNG_Init())
	{
		LCD_ShowString(Text_x,Text_Top_y*RNG_STATUS,200,16,16, "RNG Init failed");
		delay_ms(20);
		LCD_ShowString(Text_x,Text_Top_y*RNG_STATUS,200,16,16, "Rebooting");
	}
	LCD_ShowString(Text_x,Text_Top_y*RNG_READY,200,16,16, "RNG Ready");
	LCD_ShowString(Text_x,Text_Top_y*RNG_STATUS,200,16,16, "Press Key0 to get a random number");
	sprintf((char*)lcd_id,"LCD ID:%04x",lcddev.id);//将LCD ID打印到lcd_id数组。	
	LCD_ShowString(Text_x,Text_Top_y*W25Q128_STATUS,200,16,16,"W25Q128 Ready!"); 
	Flash_Write(FLASH_SAVE_ADDR,(u32*)Flash_Text,Wordlen);
	Flash_Read(FLASH_SAVE_ADDR,(u32*)Flashdata,Wordlen);
	LCD_ShowString(Text_x,Text_Top_y*FLASH_READ,200,16,16,"Flash read:"); 
	LCD_ShowString(Text_x+8*11,Text_Top_y*FLASH_READ,200,16,16,Flashdata);//显示读到的字符串
	POINT_COLOR=BLACK; 
	//DMA-uart
	j=sizeof(TEXT_TO_SEND);	   
	for(i=0;i<SEND_BUF_SIZE;i++)//填充ASCII字符集数据
    {
		if(t>=j)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}else 
			{
				SendBuff[i]=0x0d;
				mask++;
			}	
		}else//复制TEXT_TO_SEND语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}   	   
    }
	
}
