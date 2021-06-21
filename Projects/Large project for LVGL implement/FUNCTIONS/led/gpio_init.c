#include "gpio_init.h"
#include "stm32f4xx.h"
#include "usart.h"	
#include "sys.h"
#include "delay.h"

/***********TYPE DEFINE****************/
GPIO_InitTypeDef GPIO_Config;
TIM_OCInitTypeDef Timer_OCInit;
TIM_TimeBaseInitTypeDef TimeBase_Init;
USART_InitTypeDef Usart_init;
NVIC_InitTypeDef NVIC_init;
EXTI_InitTypeDef EXTI_init;
TIM_ICInitTypeDef InputCapture_init;
/***********VARIABLE DEFINE************/
u8 WWDG_CNT=0x7f;
u32 tpad_average;
/***********Constant variable**********/
#define TPAD_GATE_VAL 	100
#define TPAD_ARR_MAX_VAL  0XFFFFFFFF  //define don't need ";"
/**************************************/

void GPIO_Conf(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	//GPIO_SetBits(GPIOF, GPIO_Pin_7);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOE, &GPIO_Config);
}

void Timer_PWM_Init(u32 arr, u16 psc)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_9;
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;//need to change to AF
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOF, &GPIO_Config);
	
	TimeBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_Init.TIM_Period = arr;
	TimeBase_Init.TIM_Prescaler = psc; //change the TIM clock divition
	TIM_TimeBaseInit(TIM14, &TimeBase_Init);
	TIM_TimeBaseInit(TIM2, &TimeBase_Init);
	
	Timer_OCInit.TIM_OCMode =  TIM_OCMode_PWM1; //PWM1:when up-counting TIMx_CNT<TIMx_CCR1, it is valid PWM2:oppsite
	Timer_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;//  valid value is High
	Timer_OCInit.TIM_OutputState = TIM_OutputState_Enable;
	//Timer_init.TIM_Pulse = 100;
	TIM_OC1Init(TIM14, &Timer_OCInit);
	TIM_OC1Init(TIM2, &Timer_OCInit);
	
	
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void usart_init(u32 baud_rate)
{
	//Open necessary clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP; //the down pulse means star transmition
	GPIO_Init(GPIOA, &GPIO_Config);
	
	Usart_init.USART_BaudRate = baud_rate;
	Usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_init.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	Usart_init.USART_Parity = USART_Parity_No;
	Usart_init.USART_StopBits = USART_StopBits_1;
	Usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &Usart_init);
	USART_Cmd(USART1, ENABLE);
	

#if EN_USART1_RX
	 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_init.NVIC_IRQChannel = USART1_IRQn;//found from Stm32f4xx.h
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_init);
#endif
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		u8 res = USART_ReceiveData(USART1);
		USART_SendData(USART1, res);
	}
}

void EXTI4_IRQHandler(void)	// change the LED (the one without timer)
{
	delay_ms(10);
	if(!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))
	{
		static u8 t = 0;
		t = !t;
		if(t == 0)GPIO_ResetBits(GPIOF,GPIO_Pin_10);
		else GPIO_SetBits(GPIOF,GPIO_Pin_10);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void External_Interrupt_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_4;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_Config);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	EXTI_init.EXTI_Line = EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_init.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_init.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_init.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_init);
	
	NVIC_init.NVIC_IRQChannel = EXTI4_IRQn;//found from Stm32f4xx.h
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init);
	
}

// #descreption: 
//	1:cancel the wright protection 
//  2:set the prescaller number to get a clock
//	3:set iwdg reload value to confirm the reset time
//	4:enable IWDG and wright a function to feed dog
//	Time out = ((4*2^prer)*rlr)/32
void IWDG_Init(u8 prer, u32 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);						//32K can be devided from 2^2-2^8
	IWDG_SetReload(rlr);							//the reload value shoud < 0xfff (1111 1111 1111)=4096
	IWDG_ReloadCounter();							//to wright AAAA to KR(feed dog)
	IWDG_Enable();
}

void WWDG_Init(u8 tr, u8 wr, u32 fprer)				//fWWDG=PCLK/(4096*2^fprer) PCLK1 = 42MHz generally  
{													//tr= T[6:0], calculate value | wr= W[6,0] window value | fprer = WDGTB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_CNT = tr&WWDG_CNT;		//the tr value tshould be small than 0x7f
	WWDG_SetPrescaler(fprer);
	WWDG_SetWindowValue(wr);	//the feed dog action must be between wr-0x40, or it will reset the system; 
	//WWDG_SetCounter(WWDG_CNT);//(to prevent generating an immediate reset)
	WWDG_Enable(WWDG_CNT);   	//0x7f = 0111 1111  0x40 = 0100 0000
	
	NVIC_init.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init);
	
	WWDG_ClearFlag();
	WWDG_EnableIT();//Enable the early interrupt  when the WWDG_CNT reduced to 0x40, then the early wake is triggerd
}

void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(WWDG_CNT);
	WWDG_ClearFlag();
	LED01;
	delay_ms(3000);
	LED00;
}

void General_Timer_Interrupt(u16 arr, u16 psc)  // cnt ARR
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TimeBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_Init.TIM_Period = arr;
	TimeBase_Init.TIM_Prescaler = psc; //change the TIM clock divition
	//TimeBase_Init.TIM_RepetitionCounter = 0; //which=1 means 1 more counting then the interrupt triggered
	TIM_TimeBaseInit(TIM3, &TimeBase_Init);
	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
	NVIC_init.NVIC_IRQChannel = TIM3_IRQn;//found from Stm32f4xx.h
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init);
	
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))
	{
		LED00;
		delay_ms(500);
		LED01;
	}
	else LED01;
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
}

void CapacitiveTouch_Init(u16 psc)
{
	u16 array[10];
	u16 temp;
	u8 i,j;
	for(i=0;i<10;i++)
	{
		array[i] = TPAD_Get_Val();
		delay_ms(10);
	}
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(array[i]>array[j])
			{
				temp = array[j];
				array[j] = array[i];
				array[i] = temp;
			}
		}
	}
	temp = 0;
	for(i=2;i<8;i++) temp+=array[i];
	tpad_average = temp/6;
	printf("tpad value is:%d\n", tpad_average);
	if(tpad_average >TPAD_ARR_MAX_VAL/2) printf("initialization faild");

	//GPIOA init
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_OType= GPIO_OType_PP;
	GPIO_Config.GPIO_Pin  = GPIO_Pin_5;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Config.GPIO_Speed= GPIO_Speed_100MHz; //related to the power consumption and reaction speed;
	GPIO_Init(GPIOA, &GPIO_Config);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,GPIO_AF_TIM2);
	
	//TIMER2_Init
	TimeBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_Init.TIM_Period = TPAD_ARR_MAX_VAL;
	TimeBase_Init.TIM_Prescaler = psc-1;
	TimeBase_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TimeBase_Init);
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//Enable the interrupt and capture
	
	//Input capture init
	InputCapture_init.TIM_Channel = TIM_Channel_1;
	InputCapture_init.TIM_ICFilter= 0x00;
	InputCapture_init.TIM_ICPolarity= TIM_ICPolarity_Rising;
	InputCapture_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	InputCapture_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &InputCapture_init);

	
	//Timer priority setting
	NVIC_init.NVIC_IRQChannel = TIM2_IRQn;//found from Stm32f4xx.h
	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_init);

}

void TIM2_IRQHandler(void)
{
	printf("No response");
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC1);
}

void TPAD_Reset(void)
{
	GPIO_Config.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOA, &GPIO_Config);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	
	delay_ms(5);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
	TIM_SetCounter(TIM2,0);
	
	GPIO_Config.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Config.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_Config);
}

u16 TPAD_Get_Val(void)
{
	TPAD_Reset();
	while(TIM_GetFlagStatus(TIM2, TIM_IT_CC1) == RESET)
	{
		if(TIM_GetCounter(TIM2)>TPAD_ARR_MAX_VAL-500)
		return TIM_GetCounter(TIM2);
	}
	return TIM_GetCapture1(TIM2);
}

u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0; //enable touch
	u8 res=0;
	u8 sample=3;
	u16 rval;
	if(mode)
	{
		sample=6;
		keyen=0;
	}
	rval=TPAD_Get_MaxVal(sample);
	if(rval>(tpad_average+TPAD_GATE_VAL)&&(keyen==0))
	{
		res=1;
		keyen = 0;
	}
	
	return res;
}
	
u16 TPAD_Get_MaxVal(u8 sampt)
{
	u16 temp=0;
	u16 res=0;
	while(sampt--)
	{
		temp = TPAD_Get_Val();
		if(temp>res) res = temp;
	}
	return res;
}
