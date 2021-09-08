#include "Motor_control.h"
#include "common.h"
#include "Function_define.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Delay.h" 
#include "SFR_Macro.h"

#define Vref  3072;
#define set_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON|=SET_BIT0 ;EA=BIT_TMP
#define set_IAPGO BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;IAPTRG|=SET_BIT0 ;EA=BIT_TMP
#define clr_IAPEN BIT_TMP=EA;EA=0;TA=0xAA;TA=0x55;CHPCON&=~SET_BIT0;EA=BIT_TMP
////////////Variables////////////////
double bgvalue, ADCValue, bgvol, ADC_Vol;
uint8_t  bgmark;
uint8_t  bgh;
uint8_t  bgl;



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
	clr_P00, clr_P10;
	clr_P12; //LED on
	
	
	set_P14; //Enable DCDC
	
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
		Timer0_Delay1ms(10);
	}
}

UINT16 Get_HallValue(void)
{

	clr_ADCF;
	set_ADCS;
	ADCValue = (ADCRH<<4)+ADCRL;
//	printf("ADC value:%d",ADCValue);
	ADC_Vol = (bgvol*ADCValue/bgvalue);//All are decimal
//	printf("ADC_voltage:%gmV\n",ADC_Vol);//%g don't print no meaning 0
	return ADC_Vol;
}

void PWM_Init()
{
	PWM5_P03_OUTPUT_ENABLE;
	PWM4_P01_OUTPUT_ENABLE;//Upper bridge
	PWM_CLOCK_DIV_8;
	PWM_IMDEPENDENT_MODE;
	PWM4_OUTPUT_INVERSE;
	PWMPH = 0x00;  //Period setting;
	PWMPL = 0x96;	//13.3KHz
	PWM5H = 0x00;
	PWM5L = 0x00;
	PWM4H = 0x00;
	PWM4L = 0x00;
	/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/
}

void PWM_Setting(UINT16 n)	//1n = 1%
{
	set_SFRPAGE; //PWM4\5 SETTING
//	PWM4_P01_OUTPUT_ENABLE; // Upper bridge
	printf("ADC value:%d",ADCValue);
	printf("ADC_voltage:%gmV\n",ADC_Vol);
	PWM4H = (0xff00&n)>>8;//Lower bridge P01
	if(n>100) PWM4L = 0x96 ;
	else if(n==0) 	PWM4L = 0X96;  //Upper bridge set to low when pedal lower than 1.0V
	else PWM4L = (n*3/2);

	PWM5H = PWM4H;
	PWM5L = PWM4L;

 	set_LOAD;
	set_PWMRUN;
}

//void PinInterrupt (void) interrupt 7
//{
//	printf("Falling edge intterrupt triggered");
//}