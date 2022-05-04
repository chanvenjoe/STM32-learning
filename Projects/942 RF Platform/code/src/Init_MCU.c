#include "Init_MCU.h"


void MCU_Init(void){
		GPIO_Init();
		POWER_SW=1;									//Set the power ON
		Timer_Init();
		PWM_Init();
		ADC_Init();


	
 }

 
static void PWM_DEAD_TIME_VALUE(void)
{
		UINT8 deadtmphigh,deadtmplow;
		deadtmplow = 94;
		deadtmphigh = 11;
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

 static void PWM_Init(void){
		PWM5_P03_OUTPUT_ENABLE;				//Enable PWM5 
		PWM4_P01_OUTPUT_ENABLE;				//Enable PWM4
	  PWM_COMPLEMENTARY_MODE;	     //???????
//		 
		PWM5_OUTPUT_INVERSE;
//	 
		set_PDT45EN;    //????
		PWM_CLOCK_DIV_32;//PWM_CLOCK_DIV_16=3.9k;	  PWM_CLOCK_DIV_32=2k PWM_CLOCK_DIV_64=1k
		PWMPH = 0x00;
		PWMPL = 0xFF;//F0=4.1K  FF=3.9K
		//PWM_DEAD_TIME_VALUE(94);   //??????   27=2us     81=5us     94=6us
		PWM_DEAD_TIME_VALUE();
///**********************************************************************
//	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
//								= (16MHz/8)/(0x7CF + 1)
//								= 1KHz (1ms)
//***********************************************************************/	
		set_SFRPAGE;			 		     //PWM4 and PWM5 duty seting is in SFP page 1
		PWM4H = 0x00;						
		PWM4L = 0x00;
		clr_SFRPAGE;											

//-------- PWM start run--------------
    set_LOAD;
    set_PWMRUN;
 
 }
 
static void ADC_Init(void){
		Enable_ADC_BandGap; 
//		Enable_ADC_AIN0;        //P17Ω≈Ã§∞Â”Õ√≈
//		AD_CH=5;
		clr_ADCF;								//clear the ADC Flag
		set_ADCS; 							//A/D converting software start trigger
		EADC=0;                 //disable the ADC interrupt                                       	                                                                               // Each time ADC start trig signal
		// while(ADCF == 0);
}


static void Timer_Init(void){
		RH3 = RELOAD_VALUE_H;                       //initial counter values 
    RL3 = RELOAD_VALUE_L;       
    set_ET3;                                    //enable Timer3 interrupt
    set_EA;                                     //enable interrupts
    set_TR3;
		
 }
 
static void GPIO_Init(void){
		//P04 MOS Detection
			MOS_DET_INPUT_SET;	

		//P05 Current detection
			Cur_DET_SET;

		//P06 Short detection 
			Short_DET_SET;

		//P30 Forward&Backward detection
			FW_BC_ADC_SET;

		//P17 Pedal detection
			Pedal_DET_SET;
			
			
		//P15 external Power supply NC

		//P03	Lower Bridge PWM output
			LOW_MOS_PWM_OUTPUT_SET;


		//P01 Upper Bridge PWM output	
			UP_MOS_PWM_OUTPUT_SET;

		//P02 Charger detection
			CHG_DET_SET;
			
		//P00	Relay Control Forward
			RELAY_FW_SET;

		//P10	Relay Control Backward
			RELAY_BW_SET;
			
		//P11 BUS Voltage ADC
			BUS_VOL_ADC_SET;
			
		//P12 Error LED Indicator
			ERR_LED_SET;
			 
		//P13 Power Switch Detection
			POWER_SW_DET_SET;

		//P14 Power Switch Control
			POWER_SW_SET;
	
 }

 
 
