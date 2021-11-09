#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Delay.h" 
#include "SFR_Macro.h"

#define Vref  3072;
#define Ramp_up Timer0_Delay1ms(10); set_LOAD;set_PWMRUN//from 0->0x97 150 step, 10ms*150=1.5s
#define set_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=SET_BIT0 ;EA=BIT_TMP
#define set_IAPGO BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0 ;EA=BIT_TMP
#define clr_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;EA=BIT_TMP
//#define TIM1_INIT  TH0 = 0XFC
////////////Variables////////////////
double bgvalue, ADCValue, bgvol, ADC_Vol;
uint8_t  bgmark;
uint8_t  bgh;
uint8_t  bgl;
static uint8_t  flag=1;


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

void ADC_Init(void)
{
//	set_EA;
//	set_EPI;//Enable Pin interrupt
//	P13_Input_Mode;
//	
//	clr_PIPS1;
//	clr_PIPS0;//pin1.
//	set_PIT3;//cHANNEL 3 Edge triggered
//	clr_PIPEN3;//P13 falling triggered
//	set_PINEN3;
	set_P12; //LED on
	clr_P10;//BC
	P17_Input_Mode;//Hall
//	P30_Input_Mode;//Speed shift
//	P05_Input_Mode; //current value
	clr_P05;
	P01_PushPull_Mode;
	P14_PushPull_Mode;
	P14_PushPull_Mode;
	P00_PushPull_Mode;//Forward relay
	P10_PushPull_Mode;
	set_P14; //Enable DCDC
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
	printf("\nBandgap value:%d\n", bgvalue);
	printf("\nBandgap vo ltage:%dmV\n",bgvol);
	clr_IAPEN;		// turn off IAP
	
	Enable_ADC_AIN0;		//P17 Hall pedal
//	Enable_ADC_AIN4;		//P05 A_Det
//	Enable_ADC_AIN1;		//P30 Speed
	for(bgh =0;bgh<3;bgh++)
	{
		clr_ADCF;
		set_ADCS;
		while(ADCF==0);
		ADCValue = (ADCRH<<4)+ADCRL;
		printf("ADC value:%d",ADCValue);
	}
}
/* AD value= Voltage*255/5  20A=57*/
UINT16 Get_CurrentValue(void)
{
	Enable_ADC_AIN4;
	clr_ADCF;
	set_ADCS;
	while(ADCF==0);
//	ADCValue = (ADCRH<<4)+ADCRL;
//	printf("ADC value:%d",ADCValue);
	return ADCRH;
}

UINT16 Get_HallValue(void)
{
	Enable_ADC_AIN0;
	clr_ADCF;
	set_ADCS;//Enable ADC transfer
//	ADCValue = (ADCRH<<4)+ADCRL
//	ADC_Vol = (bgvol*ADCValue/bgvalue);//All are decimal
//	printf("ADCRH:%x\n",ADCRH);
//	printf("ADCRL:%d",ADCRL);
//	printf("ADC_voltage:%gmV\n",ADC_Vol);//%g don't print no meaning 0
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

void PWM_Init()
{
	PWM5_P03_OUTPUT_ENABLE;
	PWM4_P01_OUTPUT_ENABLE;//Upper bridge
	PWM4_OUTPUT_INVERSE;
//	PWM5_OUTPUT_INVERSE;	
	PWM_COMPLEMENTARY_MODE;//In this mode the dead time can work
	
	PWM_CLOCK_DIV_32;
//	PWMPH = 0x07;
//	PWMPL = 0xcf;	//1K
	PWMPH = 0x00;   //Period setting;
	PWMPL = 0xff;	//1.9KHz
	
	set_SFRPAGE;
	PWM4H = 0x00;
	PWM4L = 0xff;
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

void PWM_Setting(UINT8 n, UINT8 FB)	//1n = 1%
{
	set_SFRPAGE; //PWM4\5 SETTING
	flag = 1;
	n = n>100? 100: n;
	if(flag)
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
			}
		}
		if(PWM4L>10)
			Relay_On(FB);
		else
			Relay_Off();
		Ramp_up;// 10 = 2.55s ramp up
	}
	
}

//void PinInterrupt (void) interrupt 7
//{
//	printf("Falling edge intterrupt triggered");
//}