#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Delay.h" 
#include "SFR_Macro.h"


//#define TIM1_INIT  TH0 = 0XFC
////////////Variables////////////////
bit pwr_d=0;
double bgvalue, ADCValue, bgvol, ADC_Vol;

u8  bgmark;
u8  bgh;
u8  bgl;
static u8  flag=1;
static u8 u8TL1_Tmp;

// PWM+=KP[e(k) -e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
// e(k) the value difference of actual and setting e(k-1) the last time difference
// In this motor control we use P
// PWM = Kp[e(k) - e(k-1)]
// Cbat == the current value  IO:P05 ADC value
int Incremental_P(UINT8 Cbat, UINT8 CC_Value)//int can have negative num
{
	static UINT8 Kp=1,Bias,Last_bias;
	static float PWM;
	Bias= Cbat-CC_Value;
	PWM-= Kp*(Bias-Last_bias);// Decreasement output
	Last_bias = Bias;
	return PWM; //The percentage of decreasment
}

void System_init()
{
	Set_All_GPIO_Quasi_Mode;			//For GPIO1 output, Find in "Function_define.h" - "GPIO INIT"
	InitialUART0_Timer1(115200);
	Timer_Init();
	WTD_Init();
	Pin_Interruput_Init();
	ADC_Init();							
	PWM_Init();
}

void ADC_Init(void)/* AD value= Voltage*255/5  20A=57*/
{
	P17_Input_Mode;//Hall
	P30_Input_Mode;//Speed shift
	P05_Input_Mode; //current value
	P13_Input_Mode;
	P12_PushPull_Mode;
	P01_PushPull_Mode;
	P03_PushPull_Mode;
	P14_PushPull_Mode;
	P00_PushPull_Mode;//Forward relay
	P10_PushPull_Mode;
	
	clr_P12; //LED on
	clr_P10;//BC
	clr_P05;
	set_P14; //Enable DCDC
	set_P13;
	clr_P00, clr_P10;
	
	set_IAPEN;
	IAPAL = 0x0d; IAPAH = 0x00; IAPCN = 0x04;//0x04 =Read UID
	set_IAPGO;
	bgl = IAPFD;
	bgmark = bgl&0xf0;
	
	if(bgmark == 0x80)
	{
		bgl = bgl&0x0f;			
		IAPAL = 0x0c;
		IAPAH = 0x00; 
		set_IAPGO;
		bgh = IAPFD;
		bgvalue = (bgh<<4)+bgl;
		bgvol = bgvalue*3/4;
		bgvol = bgvol- 33;
	}
	if(bgmark==0x00)
	{
		bgl = bgl&0x0f;
		IAPAL = 0X0C;
		IAPAH = 0X00;
		set_IAPGO;
		bgh = IAPFD;
		bgvalue = (bgh<<4)+bgl;
		bgvol = bgvalue*3/4;
	}
	if(bgmark==0x90)
	{
		IAPAL = 0x0E;
		IAPAH = 0x00;
		set_IAPGO;
		bgh   = IAPFD;
		IAPAL = 0x0F;
		IAPAH = 0x00;
		set_IAPGO;
		bgl = IAPFD;
		bgl = bgl&0x0f;	
		bgvalue = (bgh<<4)+bgl;
		bgvol = bgvalue*3/4;
	}
//	printf("\nBandgap value:%d\n", bgvalue);
//	printf("\nBandgap vo ltage:%dmV\n",bgvol);
	clr_IAPEN;		// turn off IAP
	Enable_ADC_AIN0;		//P17 Hall pedal
	for(bgh =0;bgh<3;bgh++)
	{
		clr_ADCF;
		set_ADCS;
		while(ADCF==0);
		ADCValue = (ADCRH<<4)+ADCRL;
//		printf("ADC value:%d",ADCValue);
	}
}




void PWM_Init()
{
	PWM5_P03_OUTPUT_ENABLE;
	PWM4_P01_OUTPUT_ENABLE;//Upper bridge
	PWM4_OUTPUT_INVERSE;
	PWM_COMPLEMENTARY_MODE;//In this mode the dead time can work
	
	PWM_CLOCK_DIV_32;
	
//#if 0
//	PWMPH = 0x07;
//	PWMPL = 0xcf;	//1K
//#endif
	
	PWMPH = 0x00;   //Period setting;
	PWMPL = 0xff;	//1.9KHz
	
	set_SFRPAGE;
	PWM4H = 0x00;
	PWM4L = 0x00;
	clr_SFRPAGE;
	
	PWM45_DEADTIME_ENABLE;
	PWM_DEAD_TIME_VALUE(64); //31=2us dead time	 95=6us 63=4us input PDTCNT value
	set_LOAD;
	set_PWMRUN;
	/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
	=(16MHz/8)/(0x96+1)

	***********************************************************************/
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

void Movement_control(void)
{
	UINT8 i = Get_HallValue();// can use public structure or ...		The variables should be define at the first line
	UINT8 j = Get_CurrentValue();
	UINT8 k = Get_Speedvalue();
	UINT8 pwm_step = (i-51)>=0? (i-51)*2/3:0;  //return  %
	set_WDCLR;
	if(i>80)// to prevent hall initial voltage is 1.0v
	{
		Pressed
		switch(0/*j>57*/)//20A=57
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
					PWM4L=(PWM4L+Incremental_P(j, CCvalue)*3/2)>50? (PWM4L+Incremental_P(j, 20)*3/2):50;;//PWM delta value, if the 
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
			Timer1_Delay10ms(40);	
			PWM4L=7;
			set_LOAD;set_PWMRUN;
			Timer1_Delay10ms(40);
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
		set_P12;
		set_PD;
	}
}


UINT16 Get_CurrentValue(void)
{
	Enable_ADC_AIN4;
	clr_ADCF;
	set_ADCS;
	while(ADCF==0);
//	printf("ADC value:%d",ADCRH);
	return ADCRH;
}


UINT16 Get_HallValue(void)
{
	Enable_ADC_AIN0;
	clr_ADCF;
	set_ADCS;//Enable ADC transfer
//	printf("ADC_voltage:%gmV\n",ADCRH);//%g don't print no meaning 0
	while(ADCF==0);//ADC transfer done
	return ADCRH; //High 8 bits+ low 4 bits
}


UINT8 Get_Speedvalue(void)
{
	
	Enable_ADC_AIN1;
	clr_ADCF;
	set_ADCS;
	while(ADCF==0);
	if(ADCRH>100)
		return 1;
	else 
		return 0;
}


void Relay_On(UINT8 On_FB)//1= F 0 = B
{
	if(On_FB==1)
	{
		clr_P10;
		set_P00;
	}
	else
	{
		clr_P00;
		set_P10;
	}
		
}


void Relay_Off(void)
{
	clr_P00;
	clr_P10;
}
	/**********************************************************************
							Dead time setting
						DT=PDTCNT+1/Fsys  >Ton+Toff
						2us= 32/16M
	********************************** 3  5************************************/
void PWM_DEAD_TIME_VALUE(UINT16	DeadTimeData) //problem:when the deadtemphigh!=1, there is no dead time
{
	UINT8 deadtmphigh,deadtmplow;
	deadtmplow = DeadTimeData;
	deadtmphigh = DeadTimeData>>8;
	BIT_TMP = EA;
	if (deadtmphigh==0x01)
	{
		EA = 0;
		TA = 0xAA;
		TA = 0x55;
		PDTEN|=0x10;
	}
	TA = 0xAA;
	TA = 0x55;
	PDTCNT = deadtmplow;
	EA = BIT_TMP;
}

void PWM_Setting(UINT8 n, UINT8 FB)	//1n = 1%
{
	set_SFRPAGE; //PWM4\5 SETTING
	flag = 1;
	n = n>100? 100: n;
	while(flag)
	{
		UINT8 i = n*5/2;
		if(n>=0&&n<=100)
		{
			if(PWM4L<i)
			{
				PWM4L++;
			}
			else if(PWM4L==i)// jump out
			{
				flag = 0;
			}
			else
			{
				PWM4L=i;
				flag = 0;
			}
		}
		if(PWM4L>10)
			Relay_On(FB);
//		else
//			Relay_Off();
		Ramp_up;// 10ms = 2.55s ramp up
	}
	
}


void Timer0_IRS() interrupt 1
{
//	TF0 = 0;
	TH0 = HIBYTE(TH0_INIT);
	TL0 = LOBYTE(TH0_INIT);  
    if(u8TL1_Tmp++>9)
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

