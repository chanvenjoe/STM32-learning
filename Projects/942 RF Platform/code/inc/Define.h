#ifndef DefineH
#define DefineH

#define TEST_PIN_OUT	(((GPIOB->ODR & 0x00000020) == 0)?(GPIOB->ODR |0x00000020):(GPIOB->ODR & 0x00000000))

#define  RELOAD_VALUE_H  					(65536-8000)/256	//500US
#define  RELOAD_VALUE_L  					(65536-8000)%256	//500US

#define  GEAR_1											1
#define  GEAR_2											2
#define  GEAR_3											3
#define  GEAR_RV										4
#define  GEAR_NULL									0

#define  GEARREVLO									0x01
#define  GEARREVHI									0x01
#define  GEAR1LO										0x01
#define  GEAR1HI										0x01
#define  GEAR2LO										0x01
#define  GEAR2HI										0x01
#define  GEAR3LO										0x01
#define  GEAR3HI										0x01


//P04 MOS Detection
#define  MOS_DET_IO          				P04
#define  MOS_DET_INPUT_SET	    		P04_Input_Mode
#define  MOS_DET_OUTPUT_SET	    		P04_PushPull_Mode
#define  MOS_DET_INPUT_SET1	    		P04_Input_Mode
//P05 Current detection
#define  Cur_DET                 		P05
#define  Cur_DET_SET              	P05_Input_Mode     //?????---ok

//P06 Short detection 
#define  Short_DET           				P06              //??????---ok
#define  Short_DET_SET	      			P06_Quasi_Mode

//P30 Forward&Backward detection
#define  FW_BC_ADC          				P11
#define  FW_BC_ADC_SET	     				P11_Input_Mode     //????---ok

//P17 Pedal detection
#define  Pedal_DET                  P17
#define  Pedal_DET_SET              P17_Input_Mode 	

//P15 external Power supply NC


//P03	Lower Bridge PWM output
#define LOW_MOS_PWM_OUTPUT          P03// FP23  //
#define LOW_MOS_PWM_OUTPUT_SET	    P03_PushPull_Mode //    ok

//P01 Upper Bridge PWM output	
#define UP_MOS_PWM_OUTPUT           P01// FP23  //
#define UP_MOS_PWM_OUTPUT_SET	      P01_PushPull_Mode //   ok

//P02 Charger detection
#define  CHG_DET                 		P02	  /*???*///    ---ok
#define  CHG_DET_SET            		P02_Quasi_Mode	//P00_Input_Mode ---??  P02_Quasi_Mode

//P00	Relay Control Forward
#define  RELAY_FW        						P00  
#define  RELAY_FW_SET	   						P00_PushPull_Mode     //??---ok

//P10	Relay Control Backward
#define  RELAY_BW         					P10
#define  RELAY_BW_SET	   						P10_PushPull_Mode    //??---??

//P11 BUS Voltage ADC
#define  BUS_VOL_ADC          			P11
#define  BUS_VOL_ADC_SET	     			P11_Input_Mode     //????---ok

//P12 Error LED Indicator
#define  ERR_LED		           		 	P12
#define  ERR_LED_SET              	P12_PushPull_Mode     //?????--??
 
//P13 Power Switch Detection
#define  POWER_SW_DET         			P13
#define  POWER_SW_DET_SET	    			P13_Input_Mode


//P14 Power Switch Control
#define  POWER_SW         					P14
#define  POWER_SW_SET	   						P14_PushPull_Mode		

#define	 ChannelThrottle 						0
#define  ChannelFB				 					1
#define  ChannelCurrent							4
#define	 ChannelVoltage 						7

//Time Tick struct define

typedef struct{
	unsigned int Tick500us;
	unsigned int Tick5ms;
	unsigned int MotorCtrlCount;
	unsigned int PWMCount;
	unsigned int PowerSwitchCount;
	unsigned int ADCCount;
	unsigned int UserCtrlCount;
	
}SysClock_TypeDef;


typedef struct
{
	unsigned	int Count;
	unsigned	int Gear_Selected;

}	MotorControler_TypeDef;

typedef struct
{
	unsigned	int Count;
	unsigned	int Reading_ThrottleVol;
	unsigned	int Reading_FBVol;
	unsigned	int Reading_Current;
	unsigned  int Reading_Voltage;
}	ADC_TypeDef;



typedef struct{
	unsigned  int		Count;
	unsigned	char 	Gear;
	unsigned	char 	Throttle;
	unsigned	char	PWR_Button;
	unsigned	char	CHG_Port;
}	UserCtrl_TypeDef;



#endif
