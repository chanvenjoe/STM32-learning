#include "adc.h"
#include "delay.h"
#include "sys.h"

void ADC_Init_Config()
{
	GPIO_InitTypeDef GPIO_Config;
	ADC_CommonInitTypeDef  ADC_CommonInit_Structure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);// ADC not AHB2 it is APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	//ADC1 GPIO
	GPIO_Config.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_5;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOA, &GPIO_Config);
	//ADC3 GPIO PF7
	GPIO_Config.GPIO_Pin  = GPIO_Pin_7;
	GPIO_Init(GPIOF, &GPIO_Config);
	
	ADC_DeInit();

	ADC_CommonInit_Structure.ADC_DMAAccessMode 	= ADC_DMAAccessMode_Disabled;
	ADC_CommonInit_Structure.ADC_Mode			= ADC_Mode_Independent;
	ADC_CommonInit_Structure.ADC_Prescaler		=	ADC_Prescaler_Div4;
	ADC_CommonInit_Structure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInit_Structure);
	
	ADC_InitStructure.ADC_ContinuousConvMode		= DISABLE;
	ADC_InitStructure.ADC_DataAlign					= ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_ExternalTrigConv			//Interrupt trigger
	ADC_InitStructure.ADC_ExternalTrigConvEdge		= ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion			= 1;
	ADC_InitStructure.ADC_Resolution				= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode				= DISABLE;
	
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_Init(ADC3,&ADC_InitStructure);
	
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_Cmd(ADC3, ENABLE);
	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles );
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_480Cycles);
	
	ADC_TempSensorVrefintCmd(ENABLE);//internal temperature sensor
	
}

u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch)
{
	ADC_RegularChannelConfig(ADCx,ch,1,ADC_SampleTime_112Cycles);
	ADC_SoftwareStartConv(ADCx);
	
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));
	
	return ADC_GetConversionValue(ADCx);
}

u16 Get_ADC_Average(ADC_TypeDef* ADCx,u8 ch, u8 avg_num)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<avg_num;t++)
	{
 		temp_val+=Get_ADC(ADCx,ch);
	}
	return temp_val/avg_num;
}


