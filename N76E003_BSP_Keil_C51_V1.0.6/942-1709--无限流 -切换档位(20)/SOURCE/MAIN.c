/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
 //������¼

////20170619 G/S�޸ĳɵ͵�ƽ��Ч  .A-DET PORT����ȡ�� 
//20180327 �޸�Ϊ�ͺ�M92 1ȡ�����������PWM����
//2 G/S���Ź���ȡ������Ϊ��P11�����������Ƴ���1-2.5V��Ӧ����15-98�����15-60%	  
//20180505 ����ɲ����STOP��P00=STOP;�ڲ��������͵�ƽʱ��ֹͣ�����
//20180507	 P00 POWER����ɾ��
//20180508	 P00������
//20181008  //IO��һ�£�
           //�ٶȿ��ظ�Ϊ��AD�����Ұѽ���Ҳ���������⣻
           //���ӵ�ѹ���
            //GS��Ϊ�û���
//20181126 //1.����PWM�ֱ��Ϊ����->40% ��->70% ��->97%;
              //2.������ʱ�併��30%��
//20181210   	 DelayOffTM=20;�ĳ� DelayOffTM=2; PWM���ˣ���ʱ����FC&BC����͵�ƽ��ʱ�� 0.4S�� �ĳ�50MS
//20181218   ���ӵ��� 18 20 25�ֱ�ͬʱ�䱣��
        //   1 18A��2���ӣ�
       //    2 20A��5����
       //    3 25A��2����	
//20181219   M92�ͻ� ��������ʱ��ӿ���1.8��
//20181224 1.ȥ��P01�ĵ�Դ���ƹ��ܣ���Ϊ����LED;ȥ��P10�Ź��ܣ�ʹ�ó���ʽ��Դ���أ����ٹ����Դͨ�ϣ�
         //   2.P01ΪLED���ƽţ���ָʾ��Դ�͹��ϣ� 
          //  1�ߵ�ƽ��Ч��
          //  2������Դ�Ƴ�����
         // 3�������ƿ�������0.1�룬��0.1�룬����������ѭ������
         //  4Ƿѹ����������1�룬��1�룬����������ѭ������
        //  5����У�����0.1�룬��0.5�룬ѭ����
       // 6��������󣬵ƻظ�����������
        // 3.�������ֵ�Ӵ�
        //   1 25A��2���ӣ�
       //    2 30A��5����
       //    3 35A��2����	 
 
 //20190105 Ҫ�����汾ɲ�� 0��   ������ 1.2��;
           //ɲ�� 0��   ������ 1.5��;
            //ɲ�� 0��   ������ 1.8��;

 //20190225 ���ĵ��������޸�9B20 M92.doc���޸�
 
	//20190704  ȥ����λ���ϵĻ�������----ok
	//20190710  ��ɲ��PWM��2%��Ϊ3%----ok
  //20190717 ȥ��PWM_EN  ͬʱ��PWN��ΪPWM����ģʽ��P00Ϊ�������  P05Ϊ���������-----ok
	//20190725  ���뿴�Ź�����------------ok
	//20190731  ��������һ����6A����������ʱ����2min��Ϊ30s  �Ҷ����������α���2min����-----ok
 //20190802  ������ѹ�ǣ�0V--1V ��Ϊ��0V--0.5V ---------ok
 //          һ����ѹ�ǣ�1V--2.5V ��Ϊ: 0.5V--2.5V--------ok
 //          ���͵���ʱ���ɣ� 2s��Ϊ��3s  -----ok     
  //20190807  ����ÿ����λռ�ձȣ�����30% һ��30% ����60% ����100%-----------ok      //�������ǳ����İ汾  9fe7  600̨
  //20190828   ������ʱ����2us�Ӵ�6us-------ok

//20190829 //1.��������MOS�ܶ�·��⣻
           //2.A_DET��������--û�мӼ����ʱ
           //3.��̤����ӷ��Ӷ���������10ms��ƽ���䣬����Ϊ�ǽ�̤����Ч��
//20190907  //������������λ������������λ�岻���ӱ���ʱ����ֹͣɲ��ռ�ձ�3%���-----ok
//20190908  //��̤�󣬽���ʱ��10ms������50ms��Ч-----ok
//20190911  //�������ģʽ------ok     //��������50������ĳ����汾
//20190921   //��̤������ǿ̣�Ҫ�жϼ̵���������50ms���ٴ��жϼ̵����������Ϻ󣬲ſ�ʼmos��⣬������������������--ok         
//20190924     //��̤���ɿ�ʱ��Ҫ�ӷ���   ������⵽50ms�͵�ƽ����ʱ����Ϊ��̤�ɿ�---ok

//20200509   ����60A����--����--�ƿ���     
//20210405   ���Ž�̤�����
//20210416   ������ʱ���Ϊ2s
//20210426   ��������25A����
//20210508   ����Сռ�ձ���20%����30%

//20210528   1.������25A��2.����λPWM���ģ�3.Ƿѹֵ���ģ�4.���Ź��ϱ������ô���4V;5.����������ѹֵ��1V--1.3V

//20210806    ��̤�������������̵����жϻ���mos�ܼ��ǰ��

//20210815    ����һ������ת����  23A   35A  ---1.5s����

//20210902    ��������Ϊ3s   ���ռ�ձ�95   


#include "N76E003.h"
#include <string.h>
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"

#include "Function_define.h"
#include "TIME_ISP.h"
 
#define RELOAD_VALUE_H  (65536-8000)/256 //500US
#define RELOAD_VALUE_L  (65536-8000)%256//500US

  //---------------------------------------------------------
//funtion :Rx IR cortrol FANCAR
//body    :N76E003
//date    :2017 11 28
//in/output:
////20170619 G/S????????  .A-DET PORT????
//------------------------------------------

#define  STOP_TM      50//10MS*50=500	 ɲ��ʱ��500MS ,��λ10MS  50

#define  Vol_5V       245	//255 
#define  Vol_4_5V     244	//255    229=4.5

#define  Vol_3_5V   178//255*3.5/5            //��λ���ѹ����

#define  Vol_2_5V    128//255*2.5/5
#define  Vol_2V     102//255*2/5
#define  Vol_1V     26//255*1/5  51=1V   26=0.5V


#define  Vol_1_88V     96//255*1.88/5	19.2VʱIO��ѹΪ1.88V

#define  Vol16V       1//         //Ƿѹ���  ����27V
#define  Vol21_6V     161//     //32.7V  ���Ƿѹ   161


#define POWER_ON_TM	    100

#define  HALL_1000MV         66	//255*1/3.3=77     ������ѹ��1V�ĳ�1.3V

#define  HALL_4200MV         193//193   //255*2.5/3.3=193
#define  HALL_DV  	         (HALL_4200MV-HALL_1000MV) //116
//#define  HALL_DV  	        66
#define  PWM_15				 25//255*0.15   38

#define  HALL_4000MV         245	//255*1/3.3=77

#define   HALL_0500MV    26

#define  PWM_30				79//255*60
#define  PWM_30_DV           (PWM_30-PWM_15)//115

#define  PWM_40				104//255*60
#define  PWM_40_DV           (PWM_40-PWM_15)//115

#define  PWM_65				 168//255*60
#define  PWM_65_DV           (PWM_65-PWM_15)//115

#define  PWM_98				 243//255*98     255=100%     243=95%      248=97%  237=91.9%
#define  PWM_98_DV           (PWM_98-PWM_15)//212  


#define  ELE10A         42//255*0.466v/5=  10A=33   19
#define  ELE12A         50    //12.5A--5s����      44
#define  ELE15A 	    	55//255*0.757v/5        52

#define  ELE17_5A 	    61//255*0.886v/5         
#define  ELE21A 	      71//255*0.886v/5         
#define  ELE25A         82//255*1.255v/5        87
                                            
														
#define  ELE30A         78    //255*1.255v/5  98             27A---2min����    28.5A--5S����          99--20200507
#define  ELE32A         92                   //111
#define  ELE35A         100       //255*1.781v/5               113---20200507
	
#define  ELE60A        173//130//230   173---3.4V--60A


#define  ELE_1A         4
#define  ELE_2A         8//3A

//#define  PWM_15         40 //255*15/100

#define  PWM_55         140 //255*55/100
#define  PWM_80         204 //255*80/100


 #define  LED_IO		           P12
 #define  LED_SET              P12_PushPull_Mode     //����ָʾ��--�Ѹ�

 #define  LED1_IO		            P15
 #define  LED1_SET              P15_PushPull_Mode

#define  A_0_IO                  P05
#define  A_0_IO_SET              P05_Input_Mode     //��������---ok
 
#define  STOP_IO                  P02	  /*����*///    ---ok
#define  STOP_IO_SET              P02_Quasi_Mode//P00_Input_Mode ---�Ѹ�
  
	
#define  G_S_IO                  P17
#define  G_S_IO_SET              P17_Input_Mode 	
	
	
//#define  G_S_IO                  P10
//#define  G_S_IO_SET              P10_Input_Mode   

//#define  POWER_SW_IO		      P10
//#define  POWER_SW_IO_SET          P10_Quasi_Mode   

//#define  PROT            P14
//#define  PROT_SET        P14_Quasi_Mode
#define  RELY            P07
#define  RELY_SET        P07_PushPull_Mode     //���õ���IO��


#define  BACK_CTRL         P10
#define  BACK_CTRL_SET	   P10_PushPull_Mode    //����---�Ѹ�


#define  FRONT_CTRL        P00  
#define  FRONT_CTRL_SET	   P00_PushPull_Mode     //ǰ��---ok


#define PWMOUT            P03// FP23  //
#define PWMOUT_SET	       P03_PushPull_Mode //    ok

#define PWMOUTt            P01// FP23  //
#define PWMOUTt_SET	       P01_PushPull_Mode //   ok


#define  Vol_IO          P11
#define  Vol_IO_SET	     P11_Input_Mode     //��ѹ���---ok

 
#define  A_DET_IO           P06              //��·�������---ok
#define  A_DET_IO_SET	      P06_Quasi_Mode


#define  MOS_DET_IO          	P04
#define  MOS_DET_INPUT_SET	    P04_Input_Mode
#define  MOS_DET_OUTPUT_SET	    P04_PushPull_Mode


#define  SW_IO           P13
#define  SW_IO_SET	    P13_Input_Mode

#define  POWER_IO          P14
#define  POWER_IO_SET	    P14_PushPull_Mode



 unsigned char POWER_on_TM;

 unsigned char ELE_BIG_SET2;
 unsigned char ELE_BIG_SET1;
  unsigned char ELE_BIG_SET3;
  unsigned char VoltageLowWait;
 unsigned char  ShaCheTM;//2*20MS
  unsigned char LED_TM;  //��������Ƶ��0.26MS
 unsigned char LED_pwm; //�м����
 unsigned char LED_CT;//���Ĵ���X2
 unsigned char CurDanWei_AD;
 unsigned char  G_S_HighCT;
unsigned char   ELE_InterruptDISP_TM;

 bit NO_Connet_FL;

bit turn_zero_fl;
 bit  ELE40_BIG_FL;
 
 bit G_S_old_FL;
 bit  G_S_L2H_FL;
 
   bit  ELE60_BIG_FL;
	 
 unsigned char ELE40_BIG_ct;
 unsigned char	ELE40_Small_ct;

 unsigned char  ELE60_BIG_TM;

 unsigned int NO_Connet_ct;	

  bit POWER_ON_IO ;

 unsigned char  ELE_Wait_5s;
 unsigned char  VoltageLowCT;
bit  VoltageLowFL;
unsigned char AD_CH,Voltage_AD,DanWei_AD;//PWM_HallSet;
unsigned char ELE50_BIG_ct;
unsigned char	ELE50_Small_ct=0;
bit	ELE50_BIG_FL;

unsigned char  DanWei_PWM;
unsigned char PWM_Ele_Set;
unsigned int Hall_Add,HallLowCT;            
unsigned char   Hall_Add_ct;	
unsigned char Cur_ResultNew;
unsigned char  time_500ms;

unsigned char DanWei;

  unsigned char ELE13_BIG_TM;
	unsigned char ELE13_Small_ct;
	unsigned char ELE13_BIG_ct;

	bit	ELE13_BIG_FL;
	
	bit ELE35_BIG_ERR_FL;
	
bit ELE_ERR_FL;
 bit BIT_TMP;
 bit FRONT_FL;
 bit BACK_FL;
 bit HIGH_LOWS_PEED_FLold;

bit TurnHighSpeed_FL;
bit TurnHighSpeed_FL_New;

	bit CHANGE;
	bit LA;
	bit CHANGE_FL;
	bit HIGH_LOWS_PEED_FL;
	bit HIGH_LOWS_PEED;

	bit  ELE12_BIG_FL;
	bit ELE25_BIG_ERR_FL;//
	unsigned char   ELE12_BIG_TM;
	unsigned char ELE12_BIG_ct;
	unsigned char  ELE12_Small_ct;


	bit	ELE30_BIG_ERR_FL;//
	unsigned char	ELE30_BIG_TM;

  	bit	ELE20_BIG_ERR_FL;//
	unsigned char	ELE20_BIG_TM;


unsigned int	   Stop_same;
bit  Stop_FL;
bit Stop_Old;
unsigned char   ELE_BIG_TM;
 bit ELE_BIG_FL;
 bit G_S_Old;
 bit G_S_FL;
   unsigned char G_S_State;
 unsigned char  G_S_same;

bit A_DET_FL;

 bit FRONT_BACK_FL;


unsigned char   PWMADDTM;//
 
unsigned char TIME_20MS_CT;//
  

unsigned char  PWMold;//?????
unsigned char count_500US_CT;//500us
unsigned char PWM;
 
unsigned char CHANGE_Off_TM;

 unsigned char  FrontOFFTime;
unsigned char	BackOFFTime; 


unsigned char  DelayOffTM;//50MS/20MS=3 /*ɲ��ʱ��*/
unsigned char  BeginMoveBackTM;
unsigned char  BeginMoveFrontTM;
unsigned char  PWM_MAX_Set;		  
	  
 unsigned char Count1S;


unsigned char Count2_5MS;     
     
 unsigned char   G_S_FL_CT;	

 
 unsigned char  volHand_addct;
 unsigned int   volHand_add;   

  unsigned char  vol_addct;
 unsigned int   vol_add;  

 unsigned char ELE30_BIG_ct=0;
 unsigned char ELE30_Small_ct;			 
	bit		ELE30_BIG_FL;


  unsigned char   RELYDelayTM ;

 unsigned char     ELE28_BIG_ct,ELE28_BIG_TM;
 unsigned char     ELE28_Small_ct;

   unsigned char MOS_Err;

 
  bit              ELE28_BIG_FL;
  bit      ELE28_BIG_ERR_FL;

  bit FLAG_IO_PWM;

//-----------�������ӵı���
   bit qudiao_shache_FL;
   bit ceshi_zhengchang_FL;
	
	 bit yunxun_FL;
	 bit shangdian_yunxun_FL;
  unsigned char G_S_HighCT_fanjiao;
	 
   unsigned char PWM_HallSet;
	 
	 	 bit PWM_xianzhi_fl;
	 
	 unsigned char PWM_HUAN;
	 
	 	  bit SW_FL;
  unsigned char guanji_timer,guangji_1stimer,guangji_1mintimer;
	 unsigned char sw_timer,SW_yanshi;
	 bit FLAG_NMS;
	 
	 bit NO_Connet_FL1;
	 		
	 bit shache_4ms;
	
 bit  dang_23fl;
		unsigned char 		xiajiang_3dan_timer;
		unsigned char 		xiajiang_2dan_timer;




//------------------------------------------------------------------------------------
	
void time_dsp(void);


void Delay3us()		//@16MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}


//----------------����ʱ������--------------------
void PWM_DEAD_TIME_VALUE(UINT16	DeadTimeData)
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

//---------------------------------

void init(void)
{ 
    BACK_CTRL=0;
    FRONT_CTRL=0;   

	SW_FL=0;
	
	SW_IO_SET;
	NO_Connet_FL1=0;
	
	A_DET_IO_SET;
	
PWM_xianzhi_fl=0;
 STOP_IO_SET;
// G_S_IO_SET;                                     
// HIGH_LOWS_PEED_IO_SET;                
// FRONT_BACK_IO_SET ;                   
// POWER_SET;                  
// PROT_SET; 
 A_0_IO_SET;           	        
 RELY_SET;                    
 BACK_CTRL_SET;  					   
 FRONT_CTRL_SET;
  BACK_CTRL=0 ;
  FRONT_CTRL=0;   
 PWMOUT_SET;	 
PWMOUTt_SET;  
 STOP_IO_SET;	
 Vol_IO_SET;		
// POWER_SW_IO_SET; 
 LED_SET;
 LED1_SET;   
MOS_DET_INPUT_SET;	
//  POWER_ON_SET;
  //  PWM_EN_IO_SET;

		 PWM5_P03_OUTPUT_ENABLE;	
		 PWM4_P01_OUTPUT_ENABLE;
	   PWM_COMPLEMENTARY_MODE;	     //����Ϊ����ģʽ
		 
	//PWM4_OUTPUT_INVERSE;	
		PWM5_OUTPUT_INVERSE;

		set_PDT45EN;    //ʹ������
		PWM_CLOCK_DIV_32;//PWM_CLOCK_DIV_16=3.9k;	  PWM_CLOCK_DIV_32=2k PWM_CLOCK_DIV_64=1k
		PWMPH = 0x00;
		PWMPL = 0xFF;//F0=4.1K  FF=3.9K
		PWM_DEAD_TIME_VALUE(94);   //����ʱ������   27=2us     81=5us     94=6us
/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/	
		set_SFRPAGE;			 		     //PWM4 and PWM5 duty seting is in SFP page 1
		PWM4H = 0x00;						
		PWM4L = 0x00;
		clr_SFRPAGE;											

//-------- PWM start run--------------
    set_LOAD;
    set_PWMRUN;
///////////////////////////////////
    RH3 = RELOAD_VALUE_H;                       //initial counter values 
    RL3 = RELOAD_VALUE_L;       
    set_ET3;                                    //enable Timer3 interrupt
    set_EA;                                     //enable interrupts
    set_TR3;
  /////////////////////////////////	              
   Enable_ADC_BandGap; 
    Enable_ADC_AIN0;        //P17��̤������

		AD_CH=5;
		clr_ADCF;
		set_ADCS; 
		EADC=0;                                                                                                                                       // Each time ADC start trig signal
   // while(ADCF == 0);
//////////////////////////////
P0=0XFe;
//P1=0XDD;
//POWER_SW_IO=1;
 PWM=0;
CHANGE_Off_TM=0;
PWM_MAX_Set=255;
PWMold=1;
POWER_ON_IO=1; 
//PROT=0;
//IO_1=0;
//IO_2=0;
STOP_IO=1;
CHANGE_FL=1;
A_DET_FL=1;
//I_Err_LED_FL=0;
PWM_Ele_Set=128;
LED_IO=1;           //��Դָʾ��
LED1_IO=0;        //�������ģʽ--ָʾ��
POWER_on_TM=0;

qudiao_shache_FL=0;
ceshi_zhengchang_FL=0;
yunxun_FL=0;
shangdian_yunxun_FL=1;

POWER_IO_SET;
	
	POWER_IO=1;    //POWER �ϵ缴�Ǹߵ�ƽ
dang_23fl=0;
xiajiang_3dan_timer=0;
xiajiang_2dan_timer=0;
 }


 void Timer3_ISR (void) interrupt 16 
{
	   clr_TF3;
    count_500US_CT++;	
}


 void  MoveCtrl(void)
 {	 
	 
	   if(FRONT_CTRL==0&&BACK_CTRL==0) 
		 {
       if(G_S_State==1)
       {					
            G_S_State=2;
             MOS_Err=0;
					 
						MOS_DET_OUTPUT_SET;
						MOS_DET_IO=0;//���ǰ�ȷŵ�
						count_500US_CT=0;
						while(count_500US_CT<=40)//10MS=20
						{
						 ;
						}
         
              MOS_DET_INPUT_SET;
               count_500US_CT=0;
             while(count_500US_CT<=3)         //1.5ms  ת��������
               {
                 ;
               }
              count_500US_CT=0;
             while(count_500US_CT<=2)     //1ms
               {
                  if(MOS_DET_IO)              //�����⵽�Ǹߵ�ƽ  
                  {                   
                       MOS_Err=1;                //����������Ƕ�·��   ��6��
                       G_S_State=0xff;
                  }
               }
                              
               MOS_DET_OUTPUT_SET;      //ǿ���״̬
               MOS_DET_IO=1;           //���
              count_500US_CT=0;
             while(count_500US_CT<=40)    //40*05=20ms
               {					 
                  ;
               }
               MOS_DET_INPUT_SET;       //����ת��IO��  ����Ҫʱ��ת���ˣ�
              count_500US_CT=0;
             while(count_500US_CT<=2)    //���1ms
               {
                  if(MOS_DET_IO==0)     // �������ǵ͵�ƽ
                  {                     
                     MOS_Err=2;          //���ʾ���Ŷ�·����     ��7��
                     G_S_State=0xff;
                  }
               } 
					
          			 
											 
    }
  }
		 else
		 {
		      G_S_State=2;		   	 
		 }

      
  if(PWMold!=PWM)
  {
     PWMold=PWM;

		set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
		PWM4H = 0x00;						
		PWM4L = (255-PWM);
		clr_SFRPAGE;				
		 set_LOAD;
      // if(PWM==0)//>=0xfe)
		 if(PWM>=0xfe)//==0)
		 {
			PWM5_P03_OUTPUT_DISABLE;
			PWM4_P01_OUTPUT_DISABLE;
			FLAG_IO_PWM=1;
			P01=0;//��ΪIO
			P03=0;               //1
		  // PWM_EN_IO=0;
		 }
		 else
		 // if(PWM>=0xfe)//==0)
		  if(PWM==0)//>=0xfe)
		 {
			PWM5_P03_OUTPUT_DISABLE;
			PWM4_P01_OUTPUT_DISABLE;
			FLAG_IO_PWM=1;
			P01=0;//��ΪIO      //����
			P03=1;            // ����
			//PWM_EN_IO=0;
		 }
		else
		{
		      if(FLAG_IO_PWM)
            {      
               
			
			          PWM5_P03_OUTPUT_ENABLE;    //����
                  count_500US_CT=0;
									
            
						
                PWM4_P01_OUTPUT_ENABLE;     //����
			

              FLAG_IO_PWM=0;	 
            }			  			  	 
		}

  }
 }


void time_dsp(void)
{
  unsigned char temp;
	
   if(count_500US_CT)//;count_500US_CT>=10U;count_500US_CT-=10)//50*10=500US
	   {
	     count_500US_CT=0;
			  
			
		   if(G_S_HighCT<250)     //��̤  �ſ�����  ���¼���
		   G_S_HighCT++;      //0.5ms
			 
			 if(G_S_HighCT_fanjiao<250)   //��̤   ��������   �ſ�����
			 G_S_HighCT_fanjiao++;


        Count2_5MS++;
    if(Count2_5MS>=8)    //4ms�Լ�һ��
		{
		  Count2_5MS=0;
	    Count1S++;
				FLAG_NMS=1;
			   shache_4ms=1;
    if(Count1S>250)
	 {
        Count1S=0;
		    
		 if(ELE_InterruptDISP_TM)
			 ELE_InterruptDISP_TM--;
		 
       if(CHANGE_Off_TM)
	   {
	      CHANGE_Off_TM--;
		//  PROT=1;
		  }

      }
	  }
 ////////////////////////////////////////////////////           
                                                                                            
 if((MOS_Err)||(A_DET_FL==0)/*(A_DET_IO==0)*/||ELE25_BIG_ERR_FL||ELE35_BIG_ERR_FL||ELE28_BIG_ERR_FL||ELE30_BIG_ERR_FL||ELE20_BIG_ERR_FL||ELE40_BIG_FL||NO_Connet_FL||NO_Connet_FL1||ELE_InterruptDISP_TM)
 {
	ELE_ERR_FL=1;   
	TurnHighSpeed_FL=0;
	TurnHighSpeed_FL_New=0;     
	 qudiao_shache_FL=1;   //��������Ч������--ֹͣɲ��ռ�ձ����
 }
	else
 if(G_S_FL==0)
 {
	 	ELE_ERR_FL=0;
	 TurnHighSpeed_FL=0;
	TurnHighSpeed_FL_New=0;
  qudiao_shache_FL=0;      //�ٴ��زȽ�̤�������־λ-����
 }
 //-------------------------------------

	
//------------------------����--����ģʽ--����ģʽ���----------------------
	
	   if(STOP_IO)
		 {
			 shangdian_yunxun_FL=0;
		  Stop_same=0;
      Stop_FL=0;			    //�����籣��
		 }	
    
	
	if(STOP_IO==0)    //0Ϊ���ģʽ   1
			{
				Stop_same++;	
		
				if(Stop_same>4000)   //10ms=20     2s  �����������2s  ���ܲ���Ч
				{
					Stop_FL=1;	     //�����籣��
					yunxun_FL=0;
				}	
				 					
			}
	
//--------------------------------------------------------------------	
		
  if((DanWei_AD>=Vol_4_5V)||(CurDanWei_AD>=Vol_4_5V))              //��λ�岻����
  {
    if(++NO_Connet_ct>6000)//6000*0.5MS=3S   
	  { 
	    NO_Connet_FL=1;
	  }
	  
     goto NoTestAD; 
   }
   else
   {

   	     NO_Connet_ct=0;
         NO_Connet_FL=0; 
   }

 if(DanWei_AD>CurDanWei_AD)/*�ж�AD�仯�󲻴�*/
  temp= DanWei_AD-CurDanWei_AD; 
  else
  temp= CurDanWei_AD-DanWei_AD; 
  
  if(temp>=10)
   goto NoTestAD;
  
//	 if(DanWei_AD>Vol_3_5V)          //  ����>3.5v
	
	if(DanWei==3)
  {
  
    ELE_BIG_SET1=ELE30A;
    ELE_BIG_SET2=ELE35A;  
	  ELE_BIG_SET3=ELE32A;

     ShaCheTM=10;//10*20MS
     
    FRONT_BACK_FL=1;
    DanWei_PWM=0xff;//97%
	  DanWei=3; /*���ٵ�λ*/
	  HIGH_LOWS_PEED_FL=1;/*Ϊ0����*/
		
   }
  // else
	 // if(DanWei_AD>Vol_2_5V)         //����  2.5V----3.5V
	 if(DanWei==2)
  {
       
     NO_Connet_FL=0;
    ELE_BIG_SET1=ELE17_5A;
    ELE_BIG_SET2=ELE25A;
	ELE_BIG_SET3=ELE21A;

    ShaCheTM=20;//20*20MS
    
    FRONT_BACK_FL=1;
    DanWei_PWM=153;//65%=166  60%=153
	DanWei=2;   /*���ٵ�λ*/
	HIGH_LOWS_PEED_FL=0;/*Ϊ0����*/
   }
 //  else
	 // if(DanWei_AD>Vol_1V)        //һ��    
	 if(DanWei==1)
   {
      
			NO_Connet_FL=0;
			ELE_BIG_SET1=ELE10A;
			ELE_BIG_SET2=ELE15A;
			//ELE_BIG_SET3=0XFF;  ELE12A
      ELE_BIG_SET3=ELE12A;
     ShaCheTM=20;//20*20MS
     
			FRONT_BACK_FL=1;
			DanWei_PWM=76;//35%=89    76=30%
			DanWei=1;  /*���ٵ�λ*/
			HIGH_LOWS_PEED_FL=0;/*Ϊ0����*/
   }
	//else                    //����
	 if(DanWei==0)
	{
	  	
	    NO_Connet_FL=0;
     ELE_BIG_SET1=ELE10A;
     ELE_BIG_SET2=ELE15A;
	 // ELE_BIG_SET3=0XFF;
		ELE_BIG_SET3=ELE12A;
     ShaCheTM=20;//20*20MS
     
	 FRONT_BACK_FL=0;
    DanWei_PWM=76;//40%=102  76=30%
	 DanWei=0;  /*����λ*/
	 HIGH_LOWS_PEED_FL=0;/*Ϊ0����*/
	}
	
    
  PWM_Ele_Set=DanWei_PWM;
  PWM_MAX_Set=PWM_Ele_Set;//128;


NoTestAD:

if((CHANGE_Off_TM==0)&&G_S_FL&&(ELE_ERR_FL==0))           //��̤��̤��
   {
   
     if(FRONT_BACK_FL)
     {
       FRONT_FL=1;
       BACK_FL=0;
      }
     else
      {
       FRONT_FL=0;
       BACK_FL=1;
      }
   }
   else
   {	 
		  dang_23fl=0;      //�ָ�����
		 FRONT_FL=0;
		 BACK_FL=0;
   } 


	if(HIGH_LOWS_PEED_FLold!=HIGH_LOWS_PEED_FL)
	{
		HIGH_LOWS_PEED_FLold=HIGH_LOWS_PEED_FL;
		TurnHighSpeed_FL=0;
		TurnHighSpeed_FL_New=0;
		ELE_BIG_TM=0;
	}
	

///////////////////////////////////////
	   if((Stop_FL==1)||(VoltageLowFL==1))               //���ʱ���ߵ�ѹʱ
	   {
				   BeginMoveBackTM=0;
		       BeginMoveFrontTM=0;
           turn_zero_fl=0;
           G_S_FL=0;
		   
		 }
	  else
			
      if(FRONT_FL)
        {
         if(FRONT_CTRL)
          DelayOffTM=ShaCheTM;//50MS/20MS=3	// 2*20MS��ɲ��
          BeginMoveBackTM=0;
		    if(BeginMoveFrontTM<250)
		       BeginMoveFrontTM++;
	  
				 if(BackOFFTime>24)// 24*20MS���ٻ���    24
					 { 
						BACK_CTRL=0;
						FrontOFFTime=0;  
						FRONT_CTRL=1;
						RELY=1;
					 }
					 else
					 BeginMoveFrontTM=0;  
		   }
        else
        {
					if(BACK_FL)
					 { 			
					 if(BACK_CTRL)
					 DelayOffTM=ShaCheTM; // 2*20MS��ɲ��
					 BeginMoveFrontTM=0;
					 if(BeginMoveBackTM<250)
						BeginMoveBackTM++;
							if(FrontOFFTime>24)  // 24*20MS���ٻ���24
							{
										FRONT_CTRL=0;
										BackOFFTime=0;
										BACK_CTRL=1;
										RELY=1;
							}
							else
						 {
								BeginMoveBackTM=0;
						 }

					 }
					 else
					 {
							 BeginMoveBackTM=0;
					     BeginMoveFrontTM=0;
					 }   
		  }
   if((BeginMoveBackTM>100)||(BeginMoveFrontTM>100))            //50ms/0.5MS=100
      {	
             if(PWM<25)          //��С����ռ�ձ����ô�
               PWM=25;//??
				
      if(dang_23fl==0)	
			{				
	        PWMADDTM++;                                //������ʱ������
	  	if(PWMADDTM>27)//18=1.5S  //22==1.8s 11= 0.9s	   5=0.4s  3=0.2s  �˴���������ʱ��	 ÿ0.1S =1.222ֵ   12=1.2s    27=3s
		   {
			  PWMADDTM=0;
	 			
				 if((PWM>PWM_HallSet)/*&&(PWM>PWM_Ele_Set)*/&&(PWM>25))         //��С����ռ�ձ����ô�   12=1.2s
					PWM--;
						
					if((PWM<PWM_HallSet)/*&&(ELEbig25A_FL==0)*/)
						PWM++;
	   	 		   
        }
			}	 	  		  	    	 
	   }
	   else	 
       {
	         PWM=0;
       }	
	
	      	
			TIME_20MS_CT++;
		  if(TIME_20MS_CT>=41U)//20MS ???
		  {
            TIME_20MS_CT=0;	
			
        if(POWER_on_TM<=POWER_ON_TM) //2000/20=100      
		  	POWER_on_TM++;
                            //20200509
			 if(A_DET_FL==0||ELE40_BIG_FL||ELE35_BIG_ERR_FL||ELE25_BIG_ERR_FL||ELE_InterruptDISP_TM) //��·���Ƶƿ���ʱ��
        {
					
			   	if(++LED_TM>5)     //20*5=100ms   
			     {							 
						 LED_TM=0;
						 LED_IO^=1;
           }
			   }
			  else
			if(++LED_TM>12)//0.25s=20ms*12.5
			 {
			     LED_TM=0;
			     LED_pwm++;

                  if(LED_CT)
                    {                 
                       if(LED_pwm<LED_CT)
                        LED_IO^=1;
                        else
                        LED_IO=0;
                    }
                  
                 if(LED_pwm>=(LED_CT+4))
                    {
                      LED_pwm=0;
                      LED_IO=1;
                    
                    }
                 
			  }

			
              if(MOS_Err==1)      //���Ŷ�·��6��
                {
                  LED_CT=12;

                }
              else if(MOS_Err==2)    //���Ŷ�·��7��
                {
                  LED_CT=14;

                }
              else
                
			      if(NO_Connet_FL||NO_Connet_FL1)
                {
                 LED_CT=8;
                }
             else
            if(VoltageLowFL)
                {
                 LED_CT=4;
                }
             else           
                 if(ELE30_BIG_ERR_FL||ELE20_BIG_ERR_FL||ELE28_BIG_ERR_FL/*||ELE25_BIG_ERR_FL||ELE35_BIG_ERR_FL/*||ELEbig25A_FL||I_Err_LED_FL*/)
                {
                 LED_CT=6;	 //3��
                }
                 else
                if(Stop_FL)    //���ʱ���������1��
                {
									LED_CT=2;                         
                }
                 else
                {
                 LED_CT=0;							
                }
      //---------------20ms�Լ�һ��------------------------
								
				  if(DanWei_AD>Vol_3_5V)       //��λADֵ�ж�  ����
						{
							if(dang_23fl==0)
							   DanWei=3;
							
						  if(Cur_ResultNew>=86)       //����50ms  ����20A  ��Ϊ2��  ��PWM>90%    1.125V=20A=58 1.687V=30A=86
							{								                                          //25A=1.41V=73     30A=1.68V=85   35A=1.96V=100
							   xiajiang_2dan_timer++;
								 	
								 if(xiajiang_2dan_timer>=5)      //20*5=100ms����
								 {
								  //  DanWei=2;		
								 
									     PWM=76;              //PWM=30%    
									   MoveCtrl();	
									  dang_23fl=1;
									 xiajiang_2dan_timer=33;   
								 }
							}
							else
								xiajiang_2dan_timer=0;     //����20A  ��ʱ����
							
							
							
							if(Cur_ResultNew<=46)     //16A=0.9V=46   С��16A  ����3��
							{
																
								if(dang_23fl)      //����2������3��
								{
								  xiajiang_3dan_timer++;
								 if(xiajiang_3dan_timer>=50)    //20*50=2s 
								 {
								    	DanWei=3;		
									  dang_23fl=0;
									 xiajiang_3dan_timer=101;
								 }
								
								}							
							}
							else
								xiajiang_3dan_timer=0;      //����16A   ��ʱ����
																	
						}
						 else
	             if(DanWei_AD>Vol_2_5V)       //����
               {
								 dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
							    DanWei=2;		
							 }								 
							 else
	              if(DanWei_AD>Vol_1V)       //һ��
								{
									dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
								  DanWei=1;		
								}		
								else                    //����
								 {
									 dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
								   DanWei=0;			 
								 }									 
								
								
								
								
								
						 if(ELE60_BIG_FL)               //60A����������Ч��־     ������ת
						 {
							 if(++ELE60_BIG_TM>=30)       //0.02*60=1.2s
							 {
								ELE40_BIG_FL=1;  	 //�ƿ���
							  ELE60_BIG_TM=0;
								ELE_Wait_5s=80;
							 }
						 }
							else
							{
								ELE60_BIG_TM=0;
								if(ELE_Wait_5s==0)               
								ELE40_BIG_FL=0;
							} 	
							
  
       if(++time_500ms>25)//0.5s �������һ��    20ms�Լ�һ��
			 {
				 time_500ms=0;     //500ms�Լ�һ��
					   				 			 
		    
	//-----���������2min����   5s����  2s��������---------
     				
     //----------------------------2min����-----------------------------------        
			   if(ELE30_BIG_FL)
				{
										
						//----------------------��������----------------------------------------	
													
							if(ceshi_zhengchang_FL==0)	  //����ģʽ
							{
						   	if(DanWei==0||DanWei==1)       //������һ��  2min����  PWM_HallSet
									{
										if(++ELE30_BIG_TM>=240)//0.5s*240=120s   
										{
											ELE30_BIG_ERR_FL=1;  //
											ELE30_BIG_TM=0;
											ELE_Wait_5s=40;//40 = WAIT 20S
										}
								 }
							}
							if(DanWei==2)            //���� 2min����
							{
								if(++ELE30_BIG_TM>=240)//0.5s*240=120s   240
								{
									ELE30_BIG_ERR_FL=1;//
									ELE30_BIG_TM=0;
									ELE_Wait_5s=40;//WAIT 20S
								}					
							}
							
					     if(DanWei==3)    //   ����    1min
							{
								if(++ELE30_BIG_TM>=240)//0.5s*240=120s   
								{
									ELE30_BIG_ERR_FL=1;//
									ELE30_BIG_TM=0;
									ELE_Wait_5s=40;//WAIT 20S
								}					
							}
							
				}
				else
				{
					ELE30_BIG_TM=0;
				   if(ELE_Wait_5s==0)
            ELE30_BIG_ERR_FL=0;					
				}

    //---------------------------5s����-------������������-----------------------------------
			 
			 if(ELE28_BIG_FL)
				{
				if(DanWei==0||DanWei==1) 
					{					
					 if(++ELE28_BIG_TM>=90)//0.5s*90=45S
						{
							ELE28_BIG_ERR_FL=1;//
							ELE28_BIG_TM=0;
								ELE_Wait_5s=40;
						}
					}
					
						if(DanWei==2)
						{
						  if(++ELE28_BIG_TM>=60)//0.5s*60=30S
								{
									ELE28_BIG_ERR_FL=1;//
									ELE28_BIG_TM=0;
										ELE_Wait_5s=40;
								}
						}
						
						if(DanWei==3)
						{
						  if(++ELE28_BIG_TM>=40)//0.5s*120=20S
								{
									ELE28_BIG_ERR_FL=1;//
									ELE28_BIG_TM=0;
										ELE_Wait_5s=40;
								}
						}				
				}
				else
				{
					  ELE28_BIG_TM=0;
				   if(ELE_Wait_5s==0)
             ELE28_BIG_ERR_FL=0;					
				}

    //----------------------------2s����-----------------------------------
       if(ELE50_BIG_FL)
				{
					if(DanWei==0||DanWei==1)
          {						
						if(++ELE20_BIG_TM>=20)//0.5s*20=10S
						{
							ELE20_BIG_ERR_FL=1;//
							ELE20_BIG_TM=0;
							ELE_Wait_5s=40;
						}
				  }
						if(DanWei==2||DanWei==3)
						{
						  if(++ELE20_BIG_TM>=10)//0.5s*10=5S
							{
								ELE20_BIG_ERR_FL=1;//
								ELE20_BIG_TM=0;
									ELE_Wait_5s=40;
							}	
						}
												
				}
				else
				{
					ELE20_BIG_TM=0;        
				   if(ELE_Wait_5s==0)
             ELE20_BIG_ERR_FL=0;				 	
				}

    //---------------------------------------------
					 if(ELE12_BIG_FL)                                   //һ����ת��Ч��  
						 {                     
								if(++ELE12_BIG_TM>=2)// 5S/0.5*5=2.5s        // ��תʱ�䱣������
							 {
										 ELE25_BIG_ERR_FL=1;//
										 ELE12_BIG_TM=0;
										 ELE_Wait_5s=100;//40==WAIT 20S
								 }
						 }
						 else
						 {
								ELE12_BIG_TM=0;
								if(ELE_Wait_5s==0)
								ELE25_BIG_ERR_FL=0;              						
						 }
						 
						 
					 if(ELE13_BIG_FL)                       //������ת��Ч��
						 {                     
								if(++ELE13_BIG_TM>=2)// 5S/0.5*4=2s      // ��תʱ�䱣������
							 {
									 ELE35_BIG_ERR_FL=1;//
								
									 ELE13_BIG_TM=0;
									 ELE_Wait_5s=100;//40=WAIT 20S
							 }
						 }
						 else
						 {
								  ELE13_BIG_TM=0;
								 if(ELE_Wait_5s==0)
									ELE35_BIG_ERR_FL=0;      			 
						 }	 
						 
						 
						if(ELE60_BIG_FL)               //60A����������Ч��־     ������ת
						 {
							 if(++ELE60_BIG_TM>=1)       //0.5*2=1
							 {
								ELE40_BIG_FL=1;  	 //�ƿ���
							  ELE60_BIG_TM=0;
								ELE_Wait_5s=100;
							 }
						 }
							else
							{
								ELE60_BIG_TM=0;
								if(ELE_Wait_5s==0)               
								ELE40_BIG_FL=0;
							}
						 
						 
                 
					if(ELE_Wait_5s)
					ELE_Wait_5s--;

                   if(VoltageLowWait)
                    VoltageLowWait--;            //500ms�Լ�һ��
                   			
			}
			 
				
if(G_S_FL)             //��̤��
{
	
  if(G_S_old_FL)
  {
      G_S_old_FL=0;
      G_S_L2H_FL=1;    

  }
  
// POWER=1;
G_S_FL_CT=0;

  HallLowCT=0;
 if(Stop_FL==0)
 POWER_ON_IO=1;
 else
  POWER_ON_IO=0;

 }
 else
 {
  G_S_old_FL=1;


   if(++HallLowCT>15000) //5*60*1000/20=15000   
		{
			POWER_ON_IO=0;                                                                
		}

 }
                                           
 			 if(Voltage_AD<Vol16V)
			 {
			      VoltageLowCT++;                //20ms�Լ�һ�� 
				  if(VoltageLowCT>225)     //    225=4.5s      ��������ĵ͵���ʱ��
                   {            
					            VoltageLowFL=1;
                      turn_zero_fl=0;
                      VoltageLowWait=40;
                    }
			 }
			 else
			   {
			        VoltageLowCT=0;
					 if((Voltage_AD>Vol21_6V)/*&&(G_S_FL==0)*/&&(VoltageLowWait==0))
					    VoltageLowFL=0;
			   }



  if(FRONT_CTRL==0)
	  {
	     if(FrontOFFTime<250)    //250
        FrontOFFTime++; //??????
     }
	 else
      FrontOFFTime=0;

  if(BACK_CTRL==0)
	   {
	   if(BackOFFTime<250) 
        BackOFFTime++;//??????
     }
	 else
      BackOFFTime=0;
/////////////////////////////////////////////////
        if(DelayOffTM>1)
			   {
             DelayOffTM--;
				   RELYDelayTM=2;/*ɲ��2*20MS���ٹؼ̵���*/  //2
			   }
			   else     //ɲ�����ʱ�����
			   {
			   
			     if(RELYDelayTM)
			        RELYDelayTM--;
			      else			   
			   	  RELY=0;
               					
					  if(DelayOffTM)
               {
                   DelayOffTM=0;
                  PWM=0;
                 MoveCtrl();
              
							if(qudiao_shache_FL==0)			//����������Ч���������ɲ��ռ�ձ�3%
							{				
                PWM=10;           //1%=2.55     //���������ɲ��ռ�ձ�   Ŀǰ��3%
                 MoveCtrl();
							}
							else     //��������Ч��ֹͣ���ɲ��ռ�ձ�3%
							{
							   PWM=0;
                 MoveCtrl();
							}
				for(DelayOffTM=0;DelayOffTM<STOP_TM;DelayOffTM++)
				{
				         if(G_S_FL&&G_S_L2H_FL)
                   { 
                     G_S_L2H_FL=0;
                     goto ESC_STOP2;
                    }
                   
                count_500US_CT=0;
                   while(count_500US_CT<=20)//10MS=20    
                   {										  
                      if(G_S_FL&&G_S_L2H_FL)
                      { 
                      G_S_L2H_FL=0;
                      goto ESC_STOP2;
                      }
                   }		
              									
				}

			 ESC_STOP2:                 
										DelayOffTM=0;     //�ȹؼ̵���
										FRONT_CTRL=0;
										BACK_CTRL=0;
				         
				
								 count_500US_CT=0;
									while(count_500US_CT<=100)   //100*0.5=50ms  ��ʱ1  100
										{							
											if(G_S_FL&&G_S_L2H_FL)
											 { 
												 G_S_L2H_FL=0;
												goto ESC_STOP8;
												}
										}
				         
				             							
				
				 ESC_STOP8:    
									 PWM=0;         //�ٹ�PWM
									 MoveCtrl();																
              }	 
					 				 
					FRONT_CTRL=0;
					BACK_CTRL=0;		
					BeginMoveBackTM=0;
					BeginMoveFrontTM=0;
				
			    }
					 				 
			  G_S_L2H_FL=0; 
       }		
		
	   } 
		 
}

void ADC_DSP(void)
{
unsigned char ADtemp;
//static 	  unsigned char ele_ct1	;
 ADCF=0;
 ADtemp=ADCRH;
	
if(AD_CH==5)
{
  AD_CH=7;
  Enable_ADC_AIN0;    //��̤���������  P17
  clr_ADCF;
  set_ADCS; 
  EADC=0;   


     Hall_Add+=ADtemp;            
	   Hall_Add_ct++;
	   if(Hall_Add_ct>=8)//>=255)	//??32?,???? 64     //ƽ����8��
	   {
			// 	PROT=!PROT;
	   Hall_Add_ct=0;
	   Cur_ResultNew=Hall_Add>>3;//??    //��8    //Cur_ResultNew---����ֵ
	   Hall_Add=0;	
	  ///////3.3v==>5v=====			
		 
  /* ADtemp=Cur_ResultNew>>2;  
			 
   Hall_Add=Cur_ResultNew+ADtemp;       
   if(Hall_Add>0xff)
   	Hall_Add=0xff;
   Cur_ResultNew=Hall_Add;                 
   Hall_Add=0;*/
	 
	//==============================  
	    if(POWER_on_TM<POWER_ON_TM)
	  	   Cur_ResultNew=0;

	 /*  if(Cur_ResultNew>=ELE25A)
	   {
	       ELEbig25A_FL=1;
		   ele_ct1=0;
		   if(PWM>PWM_15)
		   PWM-=5;
	   }
	   else
	   {
	       //if(++ele_ct1>3)
	  	   ELEbig25A_FL=0;
	   }
		  */
    
         if((Cur_ResultNew>=73)&&(DanWei==1))     //25A=73    73=1.41V      
         {
		        ELE12_BIG_ct++;
				ELE12_Small_ct=0;
				if(ELE12_BIG_ct>5)
					ELE12_BIG_FL=1;                //һ����ת��Ч��־
         }
		 else
		 {
		    ELE12_BIG_ct=0;
			ELE12_Small_ct++;           
			if(ELE12_Small_ct>5)
			ELE12_BIG_FL=0;
		}  
								
			  if((Cur_ResultNew>=115)&&(DanWei==2))     //35A=100   1.96V     40A=115
              {
                ELE13_BIG_ct++;
                ELE13_Small_ct=0;
                if(ELE13_BIG_ct>5)
                 ELE13_BIG_FL=1;            //������ת��Ч��־
              }
              else
              {								
				ELE13_Small_ct++;           
				if(ELE13_Small_ct>5)      //30
				{
					ELE13_BIG_FL=0;
					ELE13_BIG_ct=0;
				}
              } 		

//      if(Cur_ResultNew > ELE60A&&(DanWei==3))//ELE40A)       //60A��������    173=3.4V
//			{		
//			 ELE40_BIG_ct++;
//			 ELE40_Small_ct=0;
//			 if(ELE40_BIG_ct>5)                   //������ת��Ч��־
//			 {
//				 ELE60_BIG_FL=1;				
//			 }
//			}
//			else
//			{		
//				//  RELY=~ RELY;        //Լ500us�Լ�һ��   
//				ELE40_BIG_ct=0;
//				ELE40_Small_ct++;			
//				if(ELE40_Small_ct>5)      //    3ms      
//				 ELE60_BIG_FL=0;
//			}

								
														
  if(Cur_ResultNew>ELE_BIG_SET2)
      {
            
				ELE50_BIG_ct++;
				ELE50_Small_ct=0;
				if(ELE50_BIG_ct>5)
				ELE50_BIG_FL=1;
      }
      else
        {            
          ELE50_BIG_ct=0;
            ELE50_Small_ct++;           
            if(ELE50_Small_ct>5)
                ELE50_BIG_FL=0;
        }

  
  if(Cur_ResultNew>ELE_BIG_SET1)
    {
          
      ELE30_BIG_ct++;
      ELE30_Small_ct=0;
    if(ELE30_BIG_ct>10)
          ELE30_BIG_FL=1;
    }
    else
      {
          
        ELE30_BIG_ct=0;
          ELE30_Small_ct++;           
          if(ELE30_Small_ct>5)
              ELE30_BIG_FL=0;
      }


	  if(Cur_ResultNew>ELE_BIG_SET3)
      {
            
        ELE28_BIG_ct++;
        ELE28_Small_ct=0;
      if(ELE28_BIG_ct>5)
            ELE28_BIG_FL=1;
      }
      else
        {
            
          ELE28_BIG_ct=0;
            ELE28_Small_ct++;           
            if(ELE28_Small_ct>5)
                ELE28_BIG_FL=0;
        }
   
  }
 }
  else
	 if(AD_CH==7)
	 {
	    AD_CH=4;
      Enable_ADC_AIN1;    //�ٶȿ���    P30
      clr_ADCF;
      set_ADCS; 
      EADC=0;
	// HallPWM=ADtemp;
		 
		 	//---------���Ŵ���4.8V  ֹͣ��������ʾ���ϵ���4��
		
		if(ADtemp>HALL_4000MV) 
		{
		  NO_Connet_FL1=1;
		}
		 	 	 
	  if(ADtemp>HALL_1000MV)    //���½�̤  1.33v
	  {
		
		G_S_HighCT_fanjiao=0;
   //if(FRONT_CTRL==0&&BACK_CTRL==0)    //50ms��Ч���жϼ̵����Ƿ�������  û�в������̤��Ч
	//	{
	    if(G_S_HighCT>=100) //10MS   0.5*20     100=50ms    200ms
		  {
		//	if(FRONT_CTRL==0&&BACK_CTRL==0)    //50ms��Ч���жϼ̵����Ƿ�������  û�в������̤��Ч
		//		{
	        if(turn_zero_fl==1)/*&&(G_S_State==0))*/
            {
							if(G_S_State==0)	
               G_S_State=1;
								  						  
              if(G_S_State==2)
						   G_S_FL=1;         //��̤����Ч		
					  }	
				//}
		 //}
	    }
			ADtemp=ADtemp-HALL_1000MV;      //
			if(ADtemp>HALL_DV)
			ADtemp=HALL_DV;

			if((HIGH_LOWS_PEED_FL)&&(DanWei==3))    //���ٵ�
			{
				if(dang_23fl==0)
			   PWM_HallSet=ADtemp*PWM_98_DV/HALL_DV; 
			}
			

		if((HIGH_LOWS_PEED_FL==0)&&(DanWei==2))    //����
			 PWM_HallSet=ADtemp*PWM_65_DV/HALL_DV; 

			if((HIGH_LOWS_PEED_FL==0)&&(DanWei==1))    //һ��
				PWM_HallSet=80;
			//PWM_HallSet=ADtemp*PWM_40_DV/HALL_DV; 

			if((HIGH_LOWS_PEED_FL==0)&&(DanWei==0))    //����
			PWM_HallSet=ADtemp*PWM_40_DV/HALL_DV; 

		  if(dang_23fl==0)
			PWM_HallSet+=PWM_15;	
		 
	  }
	  else         //�ſ���̤
	  {
	    if(ADtemp>HALL_0500MV)     //�Ҵ���0.5V  С��1.33V  
			{
          G_S_HighCT=0;	     
          NO_Connet_FL1=0;
			 if(G_S_HighCT_fanjiao>=100)    //50ms   ����50msΪ��  ����Ϊ���ɿ���̤
			 {
			    if((Stop_FL==0)&&(VoltageLowFL==0))
	         turn_zero_fl=1;
         
					 set_PWMRUN;       //PWM����ʹ��λ����
	        G_S_FL=0;
          G_S_State=0;
					PWM_xianzhi_fl=0;
			
			 }						 
      }
			else       //С��0.5V  ���ϵ���    
			{			
			    NO_Connet_FL1=1;			
			}
					
	  }
			
	 }
	 else
	  if(AD_CH==4)//�ٶȺͽ��˼��      
	 {
	    AD_CH=6;
      Enable_ADC_AIN7;     //��ѹ���   P11
      clr_ADCF;
      set_ADCS; 
      EADC=0;
           CurDanWei_AD=ADtemp;
            volHand_add+=ADtemp;            
           volHand_addct++;
           if(volHand_addct>=64)//>=255)  
           {           
           volHand_addct=0;
           DanWei_AD=volHand_add>>6;              //��λֵ
           volHand_add=0;  
            }
      
	  }
  	   else
	  if(AD_CH==6)//��ѹ���
	 {
	    AD_CH=5;
      Enable_ADC_AIN4;     //�������    P05        
      clr_ADCF;
      set_ADCS; 
      EADC=0;
      // Voltage_AD=ADtemp;

		   vol_add+=ADtemp;            
           vol_addct++;
           if(vol_addct==0)//>=64)//>=255)   //??32?,???? 64
           {
              
           vol_addct=0;
           Voltage_AD=vol_add>>8;//??
           vol_add=0;  
            }

	  }

}

void zidongguangji()
{
	if(FLAG_NMS)
	{
     FLAG_NMS=0;     //4ms�Լ�һ��
	
			if(SW_FL)
				{
						if(SW_IO)
					 {
						 SW_yanshi++;
						 if(SW_yanshi>=5)   //��������20ms
						 {						 
						   if(SW_IO)
							 {
						    	POWER_IO=0;
								 SW_yanshi=5;
							 }
					   }		 
					 }
					                  						 
				}

  	 if(SW_IO==0)
	   {  
			 
			  SW_yanshi=0;      //�尴������
		       sw_timer++;
			 
		    if(sw_timer>=10)    //40ms     //�ϵ�40ms�������ȥ�ػ�
					{
						sw_timer=0;
						SW_FL=1;	
					}

		   if(G_S_IO==0)
		   {
				if(STOP_IO)        //�����ʱ���������ʱ    
				{
			    guanji_timer++;
	            
				 if(guanji_timer>=250)
				 {
					 guanji_timer=0;				
						guangji_1stimer++;
					   if(guangji_1stimer>=60)      //1min
					   {
							 guangji_1stimer=0;
							  guangji_1mintimer++;
							    if(guangji_1mintimer>=10)
									{
									 POWER_IO=0;   
									}		                
					   }
				 }
			  }
		   }
		   else
		   {			 
             guanji_timer=0;
			       guangji_1stimer=0;
				    guangji_1mintimer=0;
		   }
		}
}
}


//-----------------???-----------------------------
void main(void)
{ 
  init();
 while(1)
  {  
		
		
		//--------���Ź�����------
      TA=0xAA;
      TA=0x55;
      WDCON = 0x7; 
     set_WDTR;
     set_WDCLR;
     set_EWDT;         //WDT���Ź�ʹ��λ   
    
		PICON=0X80;			  //���ش�����IOѡ��   P06
		PINEN=0X40;				 //ʹ��--�½��ش���
		EIE=0X02;
					
		 zidongguangji();   //�Զ��ػ�����
		//---------------------------		
    time_dsp();  
    MoveCtrl();
    if(ADCF)
    ADC_DSP();
 
  }
}


void init_ISR()  interrupt 7 
{
    if(PIF==0X40)   //P06 �ܽ��жϱ�־λ   ��Чλ 
		{		 
				Delay3us();
			if(A_DET_IO==0)
			{
		  ELE_InterruptDISP_TM = 20;    //20
			ELE_ERR_FL=1;   
			TurnHighSpeed_FL=0;
			TurnHighSpeed_FL_New=0;  			
			PWM5_P03_OUTPUT_DISABLE;
			PWM4_P01_OUTPUT_DISABLE;
			FLAG_IO_PWM=1;
			P03=1;//��ΪIO    //����
			P01=0;         //����
			turn_zero_fl=0;
		  clr_PWMRUN;                      //PWM����ʹ��λ�ر�
		// A_DET_FL=0;
			PWM=0;     
			
			//	MoveCtrl();	 
		  qudiao_shache_FL=1;   //��������Ч������--ֹͣɲ��ռ�ձ����
				
				PIF=0X00; 	       //����
			}
			  PIF=0X00; 	       //����
		}
}





