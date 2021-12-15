#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "Delay.h"
#include "Motor_control.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
#define CCvalue 0x14; //change the current regulation value

#define Not_Pressed PWM5_P03_OUTPUT_DISABLE; PWM4_P01_OUTPUT_DISABLE; clr_P01; set_P03;
#define Pressed PWM5_P03_OUTPUT_ENABLE; PWM4_P01_OUTPUT_ENABLE;
#define TH0_INIT		(65536-1333)		//(65536-55536=10000)= 7.5MS @16MHz/12==1.333333M =>0.00000075s/clock	=>10000 clocks = 0.0075s= 7.5ms  1333=1ms
#define TL0_INIT        (65536-1334)
//#define TH1_INIT        25000
//#define TL1_INIT        25000

bit pwr_d=0;

UINT8 u8TH0_Tmp,u8TL0_Tmp,u8TH1_Tmp,u8TL1_Tmp=0;



/******************************************************************************
The main C function.  Program execution starts
here after stack initialization.
******************************************************************************/
void WTD_Init();
void Timer_Init();
void Pin_Interruput_Init();

void main (void)
{	
	Set_All_GPIO_Quasi_Mode;			//For GPIO1 output, Find in "Function_define.h" - "GPIO INIT"
	InitialUART0_Timer1(115200);
	Timer_Init();
	WTD_Init();
	Pin_Interruput_Init();
	ADC_Init();							
	PWM_Init();
	while(1)
	{
		UINT8 i = Get_HallValue();// can use public structure or ...		The variables should be define at the first line
		UINT8 j = Get_CurrentValue();
		UINT8 k = Get_Speedvalue();
		UINT8 pwm_step = (i-51)>=0? (i-51)*2/3:0;  //return  %
		set_WDCLR;
		if(i>52)// to prevent hall initial voltage is 1.0v
		{
			Pressed
			switch(j>57)//20A=57
			{
				case 0:
				{
					PWM_Setting(pwm_step,k);// PWM first, or the moment relay on, PWM still 0 cause big inrush
				}
				break;
				case 1:
				{
					if(PWM4L>125)// PWM>50%
					{
						j=j*0.35;// Current calculation from current shunt-> OA-> ADC j=actural current
						PWM4L=(PWM4L+Incremental_P(j, 20)*3/2)>50? (PWM4L+Incremental_P(j, 20)*3/2):50;;//PWM delta value, if the 
												//reserve for timer counting
						set_LOAD;set_PWMRUN;
	//					Relay_On(k);		//Forward Relay open
						j=0;
					}
				}
				break;
				default:
					break;
			}
		}
		else//the brake should only works when pedal released
		{
			if(P00==1||P10==1)
			{
				PWM4L=0;
				set_LOAD;set_PWMRUN;
				Timer0_Delay1ms(400);		
				PWM4L=7;
				set_LOAD;set_PWMRUN;
				Timer0_Delay1ms(400);
				Relay_Off();
				Not_Pressed
			}
			else
			{
				PWM4L=0;
				set_LOAD;set_PWMRUN;
				Relay_Off();
			}
		}
		if(pwr_d==1)
		{
			pwr_d = 0;
			set_PD;
		}
	}
}

void WTD_Init()
{
	EA =1; //Global inter_rupt enable
	TA=0xAA;TA=0x55;WDCON=0x07;  		//Setting WDT prescale 
	set_WDTR;                       //WDT run
	set_WDCLR;						//Clear WDT timer
	set_EWDT;// WTD inter_rupt enable
}

void Timer_Init()
{
	TMOD = 0x01;
	clr_T0M; //timer0 clk=Fsys/12
	TH0 = HIBYTE(TH0_INIT);
	TL0 = LOBYTE(TH0_INIT);
    set_ET0;                                    //enable Timer0 interrupt
    set_EA;                                     //enable interrupts
    set_TR0;                                    //Timer0 run
	IPH = 0X02;
	IP=0X02;
}

void Pin_Interruput_Init()
{
	PICON = 0x21;// Port1 Pin3 edge trigger
	PINEN = 0x08; //PIN3 falling/low trigger PIPEN: Rising/high trigger
	PIPEN = 0X00; 
	EIE   = 0x02; // PIN interrupt enable
	EIP   = 0x02;		//Priority  1 1 (highest)
	EIPH  = 0X02;
	set_P1S_3; // Pin3 Schmitt trigger
}

void Timer0_IRS() interrupt 1
{
//	TF0 = 0;
	TH0 = HIBYTE(TH0_INIT);
	TL0 = LOBYTE(TH0_INIT);  
    if(u8TL1_Tmp++>99)
	{
		P12 = ~P12;   
		u8TL1_Tmp=0;
	}
}

void Pin_Interruput() interrupt 7
{
	pwr_d =~ pwr_d;
	clr_PIF3;
	TR0=~TR0;
	
}
