#include "WS2811.h"

/***********TYPE DEFINE****************/
GPIO_InitTypeDef GPIO_Configg;
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

Color_Typedef LED_Buf[LEDNUM];
u32 Color_circle[] = {White, Black, Green, Red, Blue};

void WS_Send24bits(u32 color)//Be Careful about the direction of rope: Din->Dout
{
	u8 i;
//	u32 sftcolor = color;
//	sftcolor<<8;
	for(i=0;i<24;i++)
	{
		if(color & 0x00800000)
		{
			CODE_1
		}
		else CODE_0
		color<<=1;
	}
}

void WS_Color_copy(u8 No, u32 color)
{
	LED_Buf[No].RGB = color;
}

void WS_ColorSet_LED(u8 from, u8 to, u32 color)
{
	static int temp;
	if(from>to)
	{
		temp=from; from=to; to=temp;
	}
	for(temp = from;temp<to; temp++)
	{
		WS_Color_copy(temp, color);
	}
}

void WS_Refresh()
{
	u8 i;
	for( i=0;i<LEDNUM;i++)
	{
		WS_Send24bits(LED_Buf[i].RGB);
	}
	RES
}

void WS_Key_RGB(void)
{

				static u8 temp = 0;
				switch (temp)
				{
					case 0:
						WS_ColorSet_LED(0, 20, Black);
						WS_Refresh();
						WS_ColorSet_LED(8, 13, Red);
						WS_Refresh();
						break;
					case 1:
						WS_ColorSet_LED(0, 20, Black);
						WS_Refresh();
						WS_ColorSet_LED(0, 7, Blue);
						WS_Refresh();
						break;
					case 2:
						WS_ColorSet_LED(0, 20, Black);
						WS_Refresh();
						WS_ColorSet_LED(13,20, Green);
						WS_Refresh();
						break;
				}
				temp++;
				if(temp>2)
					temp = 0;
			
		
//		WS_Refresh();
		delay_ms(50);
}

u32 CapacitiveTouch_Init(u16 psc)
{
	u32 array[10];
	u16 temp;
	u8 i,j;
	Tpad_IOInit(psc);
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
//	printf("tpad value is:%d\n", tpad_average);
//	if(tpad_average >TPAD_ARR_MAX_VAL/2) //printf("initialization faild");
		
	return tpad_average;
}

void TIM2_IRQHandler(void)
{
//	printf("No response");
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC1);
}

void TPAD_Reset(void)
{
	GPIO_Configg.GPIO_Pin = GPIO_Pin_5;
	GPIO_Configg.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Configg.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOA, &GPIO_Configg);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	
	delay_ms(50);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
	TIM_SetCounter(TIM2,0);
	
	GPIO_Configg.GPIO_Pin = GPIO_Pin_5;
	GPIO_Configg.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Configg.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_Configg);
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

u8 TPAD_Scan(u8 mode,u32 tpad_average1)
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
	if(rval>(tpad_average1+TPAD_GATE_VAL)&&(keyen==0))
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

void Tpad_IOInit(u16 psc)
{
	//GPIOA init
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_Configg.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Configg.GPIO_OType= GPIO_OType_PP;
	GPIO_Configg.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_0;
	GPIO_Configg.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Configg.GPIO_Speed= GPIO_Speed_Level_3; //related to the power consumption and reaction speed; level3 == 50MHz
	GPIO_Init(GPIOA, &GPIO_Configg);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5,GPIO_AF_2); //GPIO_AF_2 TIM1/2
	
	//TIMER2_Init
	TimeBase_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBase_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBase_Init.TIM_Period = TPAD_ARR_MAX_VAL;
	TimeBase_Init.TIM_Prescaler = psc-1;
	TimeBase_Init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TimeBase_Init);
	TIM_Cmd(TIM2,ENABLE);
	
//	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);//Enable the interrupt and capture
	
	//Input capture init
	InputCapture_init.TIM_Channel = TIM_Channel_1;
	InputCapture_init.TIM_ICFilter= 0x00;
	InputCapture_init.TIM_ICPolarity= TIM_ICPolarity_Rising;
	InputCapture_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	InputCapture_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM2, &InputCapture_init);

	
//	//Timer priority setting
//	NVIC_init.NVIC_IRQChannel = TIM2_IRQn;//found from Stm32f4xx.h
//	NVIC_init.NVIC_IRQChannelPreemptionPriority =1;
//	NVIC_init.NVIC_IRQChannelSubPriority = 1;
//	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_init);
}
