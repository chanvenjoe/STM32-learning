#ifndef __MOTOE_CONTROL_H
#define __MOTOE_CONTROL_H
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define Not_Pressed PWM5_P03_OUTPUT_DISABLE; PWM4_P01_OUTPUT_DISABLE; clr_P01; set_P03;
#define Pressed PWM5_P03_OUTPUT_ENABLE; PWM4_P01_OUTPUT_ENABLE;
#define CCvalue 5 //  change the current regulation value  if it is a number, don't use ";"
#define Vref  3072;
#define Ramp_up 	/*Timer1_Delay10ms(1);*/ set_LOAD;set_PWMRUN//from 0->0x97 150 step, 10ms*150=1.5s
#define set_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=SET_BIT0 ;EA=BIT_TMP
#define set_IAPGO BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0 ;EA=BIT_TMP
#define clr_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;EA=BIT_TMP
#define TH0_INIT		(65536-1334)		//(65536-55536=10000)= 7.5MS @16MHz/12==1.333333M =>0.00000075s/clock	=>10000 clocks = 0.0075s= 7.5ms  1333=1ms
#define TH1_INIT     (65536-65536)		//50ms
#define ADC_limit	255
#define Speed3 	178
#define Speed2 	128
#define Speed1	102
#define Reverse 0


void System_init();
void ADC_Init(void);
void PWM_Init(void);
void WTD_Init();
void Timer_Init();
void Pin_Interruput_Init();
void GPIO_Init();

u16 Get_HallValue(void);
u16 Get_CurrentValue(void);
u8  Get_Speedvalue(void);
u8  Get_RPx100Msvalue(u8 x);

void Movement_control(void);
void PWM_Setting(u8 n, u8 relay);
void PWM_DEAD_TIME_VALUE(u16	DeadTimeData);
int Incremental_P(u8 Cbat, u8 CC_value);
void Relay_On(u8 On_FB);
void Relay_Off(void);
#endif