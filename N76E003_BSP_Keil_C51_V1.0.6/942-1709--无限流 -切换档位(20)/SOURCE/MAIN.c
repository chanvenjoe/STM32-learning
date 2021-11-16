/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
 //升级记录

////20170619 G/S修改成低电平有效  .A-DET PORT功能取消 
//20180327 修改为型号M92 1取消大电流增加PWM功能
//2 G/S引脚功能取消，改为由P11采样霍尔控制车速1-2.5V对应高速15-98或低速15-60%	  
//20180505 增加刹车脚STOP，P00=STOP;内部上拉；低电平时，停止输出；
//20180507	 P00 POWER功能删除
//20180508	 P00加上拉
//20181008  //IO改一下；
           //速度开关改为用AD读，且把进退也放在这里检测；
           //增加电压检测
            //GS改为用霍尔
//20181126 //1.三档PWM分别改为：低->40% 中->70% 高->97%;
              //2.缓启动时间降低30%。
//20181210   	 DelayOffTM=20;改成 DelayOffTM=2; PWM关了，延时拉低FC&BC输出低电平的时间 0.4S， 改成50MS
//20181218   增加电流 18 20 25分别不同时间保护
        //   1 18A，2分钟，
       //    2 20A，5秒钟
       //    3 25A，2秒钟	
//20181219   M92客户 把慢启动时间加快至1.8秒
//20181224 1.去掉P01的电源控制功能，改为控制LED;去掉P10脚功能；使用常闭式电源开关，不再管理电源通断；
         //   2.P01为LED控制脚，可指示电源和故障； 
          //  1高电平有效；
          //  2开机电源灯常亮；
         // 3过流，灯快闪（亮0.1秒，灭0.1秒，保护动作后，循环）；
         //  4欠压，灯闪（亮1秒，灭1秒，保护动作后，循环）；
        //  5充电中，灯亮0.1秒，灭0.5秒，循环；
       // 6解除保护后，灯回复正常长亮；
        // 3.过流检测值加大，
        //   1 25A，2分钟，
       //    2 30A，5秒钟
       //    3 35A，2秒钟	 
 
 //20190105 要三个版本刹车 0秒   慢启动 1.2秒;
           //刹车 0秒   慢启动 1.5秒;
            //刹车 0秒   慢启动 1.8秒;

 //20190225 按文档“功能修改9B20 M92.doc”修改
 
	//20190704  去掉档位板上的霍尔调节----ok
	//20190710  将刹车PWM由2%改为3%----ok
  //20190717 去掉PWM_EN  同时将PWN改为PWM互补模式（P00为下桥输出  P05为上桥输出）-----ok
	//20190725  加入看门狗程序------------ok
	//20190731  将倒档和一档在6A保护，保护时间由2min改为30s  且二档和三档任保持2min保护-----ok
 //20190802  倒档电压是：0V--1V 改为：0V--0.5V ---------ok
 //          一档电压是：1V--2.5V 改为: 0.5V--2.5V--------ok
 //          将低电检测时间由： 2s改为：3s  -----ok     
  //20190807  更改每个档位占空比：倒档30% 一档30% 二档60% 三档100%-----------ok      //到这里是出货的版本  9fe7  600台
  //20190828   将死区时间由2us加大到6us-------ok

//20190829 //1.本版增加MOS管短路检测；
           //2.A_DET大电流检测--没有加检测延时
           //3.脚踏板检测加防接抖，需连续10ms电平不变，才认为是脚踏板有效；
//20190907  //过流保护、档位电流保护，档位板不连接保护时。将停止刹车占空比3%输出-----ok
//20190908  //脚踏后，将延时由10ms加至到50ms生效-----ok
//20190911  //加入测试模式------ok     //到这里是50块样板的出货版本
//20190921   //脚踏板踩下那刻，要判断继电器，接着50ms后，再次判断继电器，无吸合后，才开始mos检测，检测正常则才允许启动--ok         
//20190924     //脚踏板松开时，要加防抖   连续检测到50ms低电平，这时才认为脚踏松开---ok

//20200509   增加60A级别--过流--灯快闪     
//20210405   油门脚踏变霍尔
//20210416   慢启动时间改为2s
//20210426   增加限流25A功能
//20210508   将最小占空比由20%改至30%

//20210528   1.限流由25A；2.各档位PWM更改；3.欠压值更改；4.油门故障保护设置大于4V;5.油门启动电压值由1V--1.3V

//20210806    脚踏快速重启（将继电器判断还在mos管检测前）

//20210815    增加一二档堵转保护  23A   35A  ---1.5s保护

//20210902    慢启动改为3s   最大占空比95   


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

#define  STOP_TM      50//10MS*50=500	 刹车时间500MS ,单位10MS  50

#define  Vol_5V       245	//255 
#define  Vol_4_5V     244	//255    229=4.5

#define  Vol_3_5V   178//255*3.5/5            //档位板电压设置

#define  Vol_2_5V    128//255*2.5/5
#define  Vol_2V     102//255*2/5
#define  Vol_1V     26//255*1/5  51=1V   26=0.5V


#define  Vol_1_88V     96//255*1.88/5	19.2V时IO电压为1.88V

#define  Vol16V       1//         //欠压检测  低于27V
#define  Vol21_6V     161//     //32.7V  解除欠压   161


#define POWER_ON_TM	    100

#define  HALL_1000MV         66	//255*1/3.3=77     启动电压由1V改成1.3V

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
#define  ELE12A         50    //12.5A--5s保护      44
#define  ELE15A 	    	55//255*0.757v/5        52

#define  ELE17_5A 	    61//255*0.886v/5         
#define  ELE21A 	      71//255*0.886v/5         
#define  ELE25A         82//255*1.255v/5        87
                                            
														
#define  ELE30A         78    //255*1.255v/5  98             27A---2min保护    28.5A--5S保护          99--20200507
#define  ELE32A         92                   //111
#define  ELE35A         100       //255*1.781v/5               113---20200507
	
#define  ELE60A        173//130//230   173---3.4V--60A


#define  ELE_1A         4
#define  ELE_2A         8//3A

//#define  PWM_15         40 //255*15/100

#define  PWM_55         140 //255*55/100
#define  PWM_80         204 //255*80/100


 #define  LED_IO		           P12
 #define  LED_SET              P12_PushPull_Mode     //故障指示灯--已改

 #define  LED1_IO		            P15
 #define  LED1_SET              P15_PushPull_Mode

#define  A_0_IO                  P05
#define  A_0_IO_SET              P05_Input_Mode     //电流检测口---ok
 
#define  STOP_IO                  P02	  /*充电脚*///    ---ok
#define  STOP_IO_SET              P02_Quasi_Mode//P00_Input_Mode ---已改
  
	
#define  G_S_IO                  P17
#define  G_S_IO_SET              P17_Input_Mode 	
	
	
//#define  G_S_IO                  P10
//#define  G_S_IO_SET              P10_Input_Mode   

//#define  POWER_SW_IO		      P10
//#define  POWER_SW_IO_SET          P10_Quasi_Mode   

//#define  PROT            P14
//#define  PROT_SET        P14_Quasi_Mode
#define  RELY            P07
#define  RELY_SET        P07_PushPull_Mode     //不用到该IO口


#define  BACK_CTRL         P10
#define  BACK_CTRL_SET	   P10_PushPull_Mode    //后退---已改


#define  FRONT_CTRL        P00  
#define  FRONT_CTRL_SET	   P00_PushPull_Mode     //前进---ok


#define PWMOUT            P03// FP23  //
#define PWMOUT_SET	       P03_PushPull_Mode //    ok

#define PWMOUTt            P01// FP23  //
#define PWMOUTt_SET	       P01_PushPull_Mode //   ok


#define  Vol_IO          P11
#define  Vol_IO_SET	     P11_Input_Mode     //电压检测---ok

 
#define  A_DET_IO           P06              //短路电流检测---ok
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
  unsigned char LED_TM;  //调到闪的频率0.26MS
 unsigned char LED_pwm; //中间变量
 unsigned char LED_CT;//闪的次数X2
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


unsigned char  DelayOffTM;//50MS/20MS=3 /*刹车时间*/
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

//-----------更改增加的变量
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


//----------------死区时间设置--------------------
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
	   PWM_COMPLEMENTARY_MODE;	     //设置为互补模式
		 
	//PWM4_OUTPUT_INVERSE;	
		PWM5_OUTPUT_INVERSE;

		set_PDT45EN;    //使能死区
		PWM_CLOCK_DIV_32;//PWM_CLOCK_DIV_16=3.9k;	  PWM_CLOCK_DIV_32=2k PWM_CLOCK_DIV_64=1k
		PWMPH = 0x00;
		PWMPL = 0xFF;//F0=4.1K  FF=3.9K
		PWM_DEAD_TIME_VALUE(94);   //死区时间设置   27=2us     81=5us     94=6us
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
    Enable_ADC_AIN0;        //P17脚踏板油门

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
LED_IO=1;           //电源指示灯
LED1_IO=0;        //进入测试模式--指示灯
POWER_on_TM=0;

qudiao_shache_FL=0;
ceshi_zhengchang_FL=0;
yunxun_FL=0;
shangdian_yunxun_FL=1;

POWER_IO_SET;
	
	POWER_IO=1;    //POWER 上电即是高电平
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
						MOS_DET_IO=0;//检测前先放电
						count_500US_CT=0;
						while(count_500US_CT<=40)//10MS=20
						{
						 ;
						}
         
              MOS_DET_INPUT_SET;
               count_500US_CT=0;
             while(count_500US_CT<=3)         //1.5ms  转变输入检测
               {
                 ;
               }
              count_500US_CT=0;
             while(count_500US_CT<=2)     //1ms
               {
                  if(MOS_DET_IO)              //如果检测到是高电平  
                  {                   
                       MOS_Err=1;                //则表明上桥是短路的   闪6下
                       G_S_State=0xff;
                  }
               }
                              
               MOS_DET_OUTPUT_SET;      //强输出状态
               MOS_DET_IO=1;           //充电
              count_500US_CT=0;
             while(count_500US_CT<=40)    //40*05=20ms
               {					 
                  ;
               }
               MOS_DET_INPUT_SET;       //这里转变IO口  不需要时间转变了？
              count_500US_CT=0;
             while(count_500US_CT<=2)    //检测1ms
               {
                  if(MOS_DET_IO==0)     // 如果检测是低电平
                  {                     
                     MOS_Err=2;          //则表示下桥短路故障     闪7下
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
			P01=0;//换为IO
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
			P01=0;//换为IO      //上桥
			P03=1;            // 下桥
			//PWM_EN_IO=0;
		 }
		else
		{
		      if(FLAG_IO_PWM)
            {      
               
			
			          PWM5_P03_OUTPUT_ENABLE;    //下桥
                  count_500US_CT=0;
									
            
						
                PWM4_P01_OUTPUT_ENABLE;     //上桥
			

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
			  
			
		   if(G_S_HighCT<250)     //脚踏  放开清零  按下计数
		   G_S_HighCT++;      //0.5ms
			 
			 if(G_S_HighCT_fanjiao<250)   //脚踏   按下清零   放开计数
			 G_S_HighCT_fanjiao++;


        Count2_5MS++;
    if(Count2_5MS>=8)    //4ms自加一次
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
	 qudiao_shache_FL=1;   //各保护起效后，允许--停止刹车占空比输出
 }
	else
 if(G_S_FL==0)
 {
	 	ELE_ERR_FL=0;
	 TurnHighSpeed_FL=0;
	TurnHighSpeed_FL_New=0;
  qudiao_shache_FL=0;      //再次重踩脚踏后，允许标志位-清零
 }
 //-------------------------------------

	
//------------------------充电口--正常模式--测试模式入口----------------------
	
	   if(STOP_IO)
		 {
			 shangdian_yunxun_FL=0;
		  Stop_same=0;
      Stop_FL=0;			    //解除充电保护
		 }	
    
	
	if(STOP_IO==0)    //0为充电模式   1
			{
				Stop_same++;	
		
				if(Stop_same>4000)   //10ms=20     2s  插入充电插座后2s  功能才生效
				{
					Stop_FL=1;	     //进入充电保护
					yunxun_FL=0;
				}	
				 					
			}
	
//--------------------------------------------------------------------	
		
  if((DanWei_AD>=Vol_4_5V)||(CurDanWei_AD>=Vol_4_5V))              //档位板不连接
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

 if(DanWei_AD>CurDanWei_AD)/*判断AD变化大不大*/
  temp= DanWei_AD-CurDanWei_AD; 
  else
  temp= CurDanWei_AD-DanWei_AD; 
  
  if(temp>=10)
   goto NoTestAD;
  
//	 if(DanWei_AD>Vol_3_5V)          //  三档>3.5v
	
	if(DanWei==3)
  {
  
    ELE_BIG_SET1=ELE30A;
    ELE_BIG_SET2=ELE35A;  
	  ELE_BIG_SET3=ELE32A;

     ShaCheTM=10;//10*20MS
     
    FRONT_BACK_FL=1;
    DanWei_PWM=0xff;//97%
	  DanWei=3; /*高速档位*/
	  HIGH_LOWS_PEED_FL=1;/*为0低速*/
		
   }
  // else
	 // if(DanWei_AD>Vol_2_5V)         //二档  2.5V----3.5V
	 if(DanWei==2)
  {
       
     NO_Connet_FL=0;
    ELE_BIG_SET1=ELE17_5A;
    ELE_BIG_SET2=ELE25A;
	ELE_BIG_SET3=ELE21A;

    ShaCheTM=20;//20*20MS
    
    FRONT_BACK_FL=1;
    DanWei_PWM=153;//65%=166  60%=153
	DanWei=2;   /*中速档位*/
	HIGH_LOWS_PEED_FL=0;/*为0低速*/
   }
 //  else
	 // if(DanWei_AD>Vol_1V)        //一档    
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
			DanWei=1;  /*低速档位*/
			HIGH_LOWS_PEED_FL=0;/*为0低速*/
   }
	//else                    //倒挡
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
	 DanWei=0;  /*倒档位*/
	 HIGH_LOWS_PEED_FL=0;/*为0低速*/
	}
	
    
  PWM_Ele_Set=DanWei_PWM;
  PWM_MAX_Set=PWM_Ele_Set;//128;


NoTestAD:

if((CHANGE_Off_TM==0)&&G_S_FL&&(ELE_ERR_FL==0))           //脚踏板踏下
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
		  dang_23fl=0;      //恢复正常
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
	   if((Stop_FL==1)||(VoltageLowFL==1))               //充电时或者低压时
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
          DelayOffTM=ShaCheTM;//50MS/20MS=3	// 2*20MS后刹车
          BeginMoveBackTM=0;
		    if(BeginMoveFrontTM<250)
		       BeginMoveFrontTM++;
	  
				 if(BackOFFTime>24)// 24*20MS后再换向    24
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
					 DelayOffTM=ShaCheTM; // 2*20MS后刹车
					 BeginMoveFrontTM=0;
					 if(BeginMoveBackTM<250)
						BeginMoveBackTM++;
							if(FrontOFFTime>24)  // 24*20MS后再换向24
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
             if(PWM<25)          //最小启动占空比设置处
               PWM=25;//??
				
      if(dang_23fl==0)	
			{				
	        PWMADDTM++;                                //慢启动时间设置
	  	if(PWMADDTM>27)//18=1.5S  //22==1.8s 11= 0.9s	   5=0.4s  3=0.2s  此处调整加速时间	 每0.1S =1.222值   12=1.2s    27=3s
		   {
			  PWMADDTM=0;
	 			
				 if((PWM>PWM_HallSet)/*&&(PWM>PWM_Ele_Set)*/&&(PWM>25))         //最小启动占空比设置处   12=1.2s
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
			 if(A_DET_FL==0||ELE40_BIG_FL||ELE35_BIG_ERR_FL||ELE25_BIG_ERR_FL||ELE_InterruptDISP_TM) //短路控制灯快闪时间
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

			
              if(MOS_Err==1)      //上桥短路闪6下
                {
                  LED_CT=12;

                }
              else if(MOS_Err==2)    //下桥短路闪7下
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
                 LED_CT=6;	 //3次
                }
                 else
                if(Stop_FL)    //充电时，进入灯闪1下
                {
									LED_CT=2;                         
                }
                 else
                {
                 LED_CT=0;							
                }
      //---------------20ms自加一次------------------------
								
				  if(DanWei_AD>Vol_3_5V)       //档位AD值判断  三档
						{
							if(dang_23fl==0)
							   DanWei=3;
							
						  if(Cur_ResultNew>=86)       //连续50ms  高于20A  减为2档  且PWM>90%    1.125V=20A=58 1.687V=30A=86
							{								                                          //25A=1.41V=73     30A=1.68V=85   35A=1.96V=100
							   xiajiang_2dan_timer++;
								 	
								 if(xiajiang_2dan_timer>=5)      //20*5=100ms连续
								 {
								  //  DanWei=2;		
								 
									     PWM=76;              //PWM=30%    
									   MoveCtrl();	
									  dang_23fl=1;
									 xiajiang_2dan_timer=33;   
								 }
							}
							else
								xiajiang_2dan_timer=0;     //低于20A  计时清零
							
							
							
							if(Cur_ResultNew<=46)     //16A=0.9V=46   小于16A  升回3档
							{
																
								if(dang_23fl)      //是由2档升入3档
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
								xiajiang_3dan_timer=0;      //大于16A   计时清零
																	
						}
						 else
	             if(DanWei_AD>Vol_2_5V)       //二档
               {
								 dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
							    DanWei=2;		
							 }								 
							 else
	              if(DanWei_AD>Vol_1V)       //一档
								{
									dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
								  DanWei=1;		
								}		
								else                    //倒档
								 {
									 dang_23fl=0;
								 xiajiang_3dan_timer=0;
								 xiajiang_2dan_timer=0;
								   DanWei=0;			 
								 }									 
								
								
								
								
								
						 if(ELE60_BIG_FL)               //60A过流保护生效标志     三档堵转
						 {
							 if(++ELE60_BIG_TM>=30)       //0.02*60=1.2s
							 {
								ELE40_BIG_FL=1;  	 //灯快闪
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
							
  
       if(++time_500ms>25)//0.5s 进入调整一次    20ms自加一次
			 {
				 time_500ms=0;     //500ms自加一次
					   				 			 
		    
	//-----在这里更改2min保护   5s保护  2s保护设置---------
     				
     //----------------------------2min保护-----------------------------------        
			   if(ELE30_BIG_FL)
				{
										
						//----------------------三级保护----------------------------------------	
													
							if(ceshi_zhengchang_FL==0)	  //正常模式
							{
						   	if(DanWei==0||DanWei==1)       //倒档和一档  2min保护  PWM_HallSet
									{
										if(++ELE30_BIG_TM>=240)//0.5s*240=120s   
										{
											ELE30_BIG_ERR_FL=1;  //
											ELE30_BIG_TM=0;
											ELE_Wait_5s=40;//40 = WAIT 20S
										}
								 }
							}
							if(DanWei==2)            //二档 2min保护
							{
								if(++ELE30_BIG_TM>=240)//0.5s*240=120s   240
								{
									ELE30_BIG_ERR_FL=1;//
									ELE30_BIG_TM=0;
									ELE_Wait_5s=40;//WAIT 20S
								}					
							}
							
					     if(DanWei==3)    //   三档    1min
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

    //---------------------------5s保护-------（二级保护）-----------------------------------
			 
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

    //----------------------------2s保护-----------------------------------
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
					 if(ELE12_BIG_FL)                                   //一档堵转生效后  
						 {                     
								if(++ELE12_BIG_TM>=2)// 5S/0.5*5=2.5s        // 堵转时间保护设置
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
						 
						 
					 if(ELE13_BIG_FL)                       //二档堵转生效后
						 {                     
								if(++ELE13_BIG_TM>=2)// 5S/0.5*4=2s      // 堵转时间保护设置
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
						 
						 
						if(ELE60_BIG_FL)               //60A过流保护生效标志     三档堵转
						 {
							 if(++ELE60_BIG_TM>=1)       //0.5*2=1
							 {
								ELE40_BIG_FL=1;  	 //灯快闪
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
                    VoltageLowWait--;            //500ms自减一次
                   			
			}
			 
				
if(G_S_FL)             //脚踏板
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
			      VoltageLowCT++;                //20ms自加一次 
				  if(VoltageLowCT>225)     //    225=4.5s      在这里更改低电检测时间
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
				   RELYDelayTM=2;/*刹车2*20MS后再关继电器*/  //2
			   }
			   else     //刹车间隔时间完后
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
              
							if(qudiao_shache_FL==0)			//各保护不起效，才能输出刹车占空比3%
							{				
                PWM=10;           //1%=2.55     //在这里更改刹车占空比   目前是3%
                 MoveCtrl();
							}
							else     //各保护起效后，停止输出刹车占空比3%
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
										DelayOffTM=0;     //先关继电器
										FRONT_CTRL=0;
										BACK_CTRL=0;
				         
				
								 count_500US_CT=0;
									while(count_500US_CT<=100)   //100*0.5=50ms  延时1  100
										{							
											if(G_S_FL&&G_S_L2H_FL)
											 { 
												 G_S_L2H_FL=0;
												goto ESC_STOP8;
												}
										}
				         
				             							
				
				 ESC_STOP8:    
									 PWM=0;         //再关PWM
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
  Enable_ADC_AIN0;    //脚踏板霍尔油门  P17
  clr_ADCF;
  set_ADCS; 
  EADC=0;   


     Hall_Add+=ADtemp;            
	   Hall_Add_ct++;
	   if(Hall_Add_ct>=8)//>=255)	//??32?,???? 64     //平均了8次
	   {
			// 	PROT=!PROT;
	   Hall_Add_ct=0;
	   Cur_ResultNew=Hall_Add>>3;//??    //除8    //Cur_ResultNew---电流值
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
					ELE12_BIG_FL=1;                //一档堵转生效标志
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
                 ELE13_BIG_FL=1;            //二档堵转生效标志
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

//      if(Cur_ResultNew > ELE60A&&(DanWei==3))//ELE40A)       //60A过流保护    173=3.4V
//			{		
//			 ELE40_BIG_ct++;
//			 ELE40_Small_ct=0;
//			 if(ELE40_BIG_ct>5)                   //三档堵转生效标志
//			 {
//				 ELE60_BIG_FL=1;				
//			 }
//			}
//			else
//			{		
//				//  RELY=~ RELY;        //约500us自加一次   
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
      Enable_ADC_AIN1;    //速度开关    P30
      clr_ADCF;
      set_ADCS; 
      EADC=0;
	// HallPWM=ADtemp;
		 
		 	//---------油门大于4.8V  停止工作，显示故障灯闪4下
		
		if(ADtemp>HALL_4000MV) 
		{
		  NO_Connet_FL1=1;
		}
		 	 	 
	  if(ADtemp>HALL_1000MV)    //踩下脚踏  1.33v
	  {
		
		G_S_HighCT_fanjiao=0;
   //if(FRONT_CTRL==0&&BACK_CTRL==0)    //50ms生效后，判断继电器是否有吸合  没有才允许脚踏生效
	//	{
	    if(G_S_HighCT>=100) //10MS   0.5*20     100=50ms    200ms
		  {
		//	if(FRONT_CTRL==0&&BACK_CTRL==0)    //50ms生效后，判断继电器是否有吸合  没有才允许脚踏生效
		//		{
	        if(turn_zero_fl==1)/*&&(G_S_State==0))*/
            {
							if(G_S_State==0)	
               G_S_State=1;
								  						  
              if(G_S_State==2)
						   G_S_FL=1;         //脚踏板生效		
					  }	
				//}
		 //}
	    }
			ADtemp=ADtemp-HALL_1000MV;      //
			if(ADtemp>HALL_DV)
			ADtemp=HALL_DV;

			if((HIGH_LOWS_PEED_FL)&&(DanWei==3))    //高速档
			{
				if(dang_23fl==0)
			   PWM_HallSet=ADtemp*PWM_98_DV/HALL_DV; 
			}
			

		if((HIGH_LOWS_PEED_FL==0)&&(DanWei==2))    //二档
			 PWM_HallSet=ADtemp*PWM_65_DV/HALL_DV; 

			if((HIGH_LOWS_PEED_FL==0)&&(DanWei==1))    //一档
				PWM_HallSet=80;
			//PWM_HallSet=ADtemp*PWM_40_DV/HALL_DV; 

			if((HIGH_LOWS_PEED_FL==0)&&(DanWei==0))    //倒档
			PWM_HallSet=ADtemp*PWM_40_DV/HALL_DV; 

		  if(dang_23fl==0)
			PWM_HallSet+=PWM_15;	
		 
	  }
	  else         //放开脚踏
	  {
	    if(ADtemp>HALL_0500MV)     //且大于0.5V  小于1.33V  
			{
          G_S_HighCT=0;	     
          NO_Connet_FL1=0;
			 if(G_S_HighCT_fanjiao>=100)    //50ms   连续50ms为低  才认为是松开脚踏
			 {
			    if((Stop_FL==0)&&(VoltageLowFL==0))
	         turn_zero_fl=1;
         
					 set_PWMRUN;       //PWM运行使能位开启
	        G_S_FL=0;
          G_S_State=0;
					PWM_xianzhi_fl=0;
			
			 }						 
      }
			else       //小于0.5V  故障灯闪    
			{			
			    NO_Connet_FL1=1;			
			}
					
	  }
			
	 }
	 else
	  if(AD_CH==4)//速度和进退检测      
	 {
	    AD_CH=6;
      Enable_ADC_AIN7;     //电压检测   P11
      clr_ADCF;
      set_ADCS; 
      EADC=0;
           CurDanWei_AD=ADtemp;
            volHand_add+=ADtemp;            
           volHand_addct++;
           if(volHand_addct>=64)//>=255)  
           {           
           volHand_addct=0;
           DanWei_AD=volHand_add>>6;              //档位值
           volHand_add=0;  
            }
      
	  }
  	   else
	  if(AD_CH==6)//电压检测
	 {
	    AD_CH=5;
      Enable_ADC_AIN4;     //电流检测    P05        
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
     FLAG_NMS=0;     //4ms自加一次
	
			if(SW_FL)
				{
						if(SW_IO)
					 {
						 SW_yanshi++;
						 if(SW_yanshi>=5)   //按键防抖20ms
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
			 
			  SW_yanshi=0;      //清按键计数
		       sw_timer++;
			 
		    if(sw_timer>=10)    //40ms     //上电40ms后才允许去关机
					{
						sw_timer=0;
						SW_FL=1;	
					}

		   if(G_S_IO==0)
		   {
				if(STOP_IO)        //不充电时进入待机计时    
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
		
		
		//--------看门狗设置------
      TA=0xAA;
      TA=0x55;
      WDCON = 0x7; 
     set_WDTR;
     set_WDCLR;
     set_EWDT;         //WDT看门狗使能位   
    
		PICON=0X80;			  //边沿触发和IO选择   P06
		PINEN=0X40;				 //使能--下降沿触发
		EIE=0X02;
					
		 zidongguangji();   //自动关机程序
		//---------------------------		
    time_dsp();  
    MoveCtrl();
    if(ADCF)
    ADC_DSP();
 
  }
}


void init_ISR()  interrupt 7 
{
    if(PIF==0X40)   //P06 管脚中断标志位   生效位 
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
			P03=1;//换为IO    //下桥
			P01=0;         //上桥
			turn_zero_fl=0;
		  clr_PWMRUN;                      //PWM运行使能位关闭
		// A_DET_FL=0;
			PWM=0;     
			
			//	MoveCtrl();	 
		  qudiao_shache_FL=1;   //各保护起效后，允许--停止刹车占空比输出
				
				PIF=0X00; 	       //清零
			}
			  PIF=0X00; 	       //清零
		}
}





