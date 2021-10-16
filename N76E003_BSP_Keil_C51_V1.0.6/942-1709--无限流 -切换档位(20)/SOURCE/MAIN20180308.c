/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

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
////20170619 G/S修改成低电平有效  .A-DET PORT功能取消
//-------------------------------------－－－－－

#define  ELE11A         40//64// 91//77//电压1V 255*0.825/3.3  75MV=1A  
#define  ELE13A         55//75//


#define  ELE18A         76// 
#define  ELE20A         85// 
#define  ELE25A         106//174//




#define  ELE50A         250//3A

#define  ELE_1A         4
#define  ELE_2A         8//3A

#define  PWM_55         140 //255*55/100
#define  PWM_80         204 //255*80/100

#define  A_0_IO                  P04
#define  A_0_IO_SET              P04_Input_Mode 
   
#define  G_S_IO                  P10
#define  G_S_IO_SET              P10_Input_Mode   
#define  HIGH_LOWS_PEED_IO       P05
#define  HIGH_LOWS_PEED_IO_SET   P05_Quasi_Mode
#define  FRONT_BACK_IO           P12
#define  FRONT_BACK_IO_SET       P12_Quasi_Mode

#define  POWER           P00
#define  POWER_SET       P00_PushPull_Mode
#define  PROT            P14
#define  PROT_SET        P14_Quasi_Mode
#define  RELY            P03
#define  RELY_SET        P03_PushPull_Mode
#define  BACK_CTRL         P06
#define  BACK_CTRL_SET	   P06_PushPull_Mode
#define  FRONT_CTRL        P07  
#define  FRONT_CTRL_SET	   P07_PushPull_Mode

#define  PWMOUT            P15// FP23
 #define PWMOUT_SET	       P15_PushPull_Mode

#define  A_DET_IO          P17
#define  A_DET_IO_SET	     P17_Quasi_Mode
//#define  PROT              P14

unsigned char ELE50_BIG_ct;
unsigned char	ELE50_Small_ct=0;
bit	ELE50_BIG_FL;
unsigned char ELE50_BIG_TM;

unsigned char PWM_Ele_Set;
unsigned int Hall_Add;            
unsigned char   Hall_Add_ct;	
unsigned char Cur_ResultNew;
unsigned char  time_500ms;
unsigned char AD_Max,AD_Min;
unsigned char AD_Max_Index,AD_Min_Index;
//unsigned char  Temp;
  unsigned char ELE13_BIG_TM;
	unsigned char ELE13_Small_ct;
	unsigned char ELE13_BIG_ct;
	unsigned char ELE_set0;
	bit	ELE13_BIG_FL;
	
bit ELE_ERR_FL;
 bit BIT_TMP;
 bit FRONT_FL;
 bit BACK_FL;
 bit HIGH_LOWS_PEED_FLold;
 // bit FRONT_NEW_FL;
 //bit BACK_NEW_FL;
bit TurnHighSpeed_FL;
bit TurnHighSpeed_FL_New;
 bit  IO_1;
  bit IO_2;
  bit  CHANGE_IO;


 bit FRONT_FL;
 bit BACK_FL;
 // bit FRONT_NEW_FL;
 //bit BACK_NEW_FL;
unsigned char ELE_Small_ct;
 bit  CHANGE;
 bit  LA;
  bit CHANGE_FL;
  bit HIGH_LOWS_PEED_FL;
  bit HIGH_LOWS_PEED;
	bit	ELE30_BIG_ERR_FL;//
	unsigned char	ELE30_BIG_TM;
// unsigned char BACK_FRONT_Old; 	
//unsigned char BACK_FRONT_New; 
//unsigned char BACK_FRONT_same;
unsigned char  ELE_5S[8];//每半秒一个AD值

unsigned char HIGH_LOW_same;
unsigned char  CHANGE_same;
unsigned char   ELE_BIG_TM;
 bit ELE_BIG_FL;
 bit G_S_Old;
 bit G_S_FL;
 unsigned char  G_S_same;
 bit A_DET_Old;
 bit A_DET_FL;
 unsigned char  A_DET_same;

 bit FRONT_BACK_Old;
 bit FRONT_BACK_FL;
 unsigned char  FRONT_BACK_same;

unsigned char   PWMADDTM;//
 
unsigned char TIME_20MS_CT;//
  
//unsigned char TIME_Dance_CT;
unsigned char  temp;
unsigned char  PWMold;//跳舞隔时间
unsigned char count_500US_CT;//500us
unsigned char PWM;
 
unsigned char CHANGE_Off_TM;

 unsigned char  FrontOFFTime;
unsigned char	BackOFFTime; 


unsigned char  DelayOffTM;//50MS/20MS=3
unsigned char  BeginMoveBackTM;
unsigned char  BeginMoveFrontTM;
unsigned char  PWM_MAX_Set;		  
	  
unsigned char Count1S;
 unsigned char ELE_BIG_ct;

unsigned char PROT_High_ct;
unsigned char Count2_5MS;           
unsigned char	 G_S_FL_CT1;
 unsigned long   G_S_FL_CT;	
 
 unsigned char ELE30_BIG_ct=0;
 unsigned char ELE30_Small_ct;			
	 
	bit		ELE30_BIG_FL;
 
 
 #if 0
 code unsigned char PWM_ELE_Tab[248] = //256
	{
	    0x00,0x1D,0x3A,0x27,0x74,0x69,0x4E,0x53,
	    0xE8,0xF5,0xD2,0xCF,0x9C,0x81,0xA6,0xBB,
	    0xCD,0xD0,0xF7,0xEA,0xB9,0xA4,0x83,0x9E,
	    0x25,0x38,0x1F,0x02,0x51,0x4C,0x6B,0x76,
	    0x87,0x9A,0xBD,0xA0,0xF3,0xEE,0xC9,0xD4,
	    0x6F,0x72,0x55,0x48,0x1B,0x06,0x21,0x3C,
	    0x4A,0x57,0x70,0x6D,0x3E,0x23,0x04,0x19,
	    0xA2,0xBF,0x98,0x85,0xD6,0xCB,0xEC,0xF1,
	    0x13,0x0E,0x29,0x34,0x67,0x7A,0x5D,0x40,
	    0xFB,0xE6,0xC1,0xDC,0x8F,0x92,0xB5,0xA8,
	    0xDE,0xC3,0xE4,0xF9,0xAA,0xB7,0x90,0x8D,
	    0x36,0x2B,0x0C,0x11,0x42,0x5F,0x78,0x65,
	    0x94,0x89,0xAE,0xB3,0xE0,0xFD,0xDA,0xC7,
	    0x7C,0x61,0x46,0x5B,0x08,0x15,0x32,0x2F,
	    0x59,0x44,0x63,0x7E,0x2D,0x30,0x17,0x0A,
	    0xB1,0xAC,0x8B,0x96,0xC5,0xD8,0xFF,0xE2,
	    0x26,0x3B,0x1C,0x01,0x52,0x4F,0x68,0x75,
	    0xCE,0xD3,0xF4,0xE9,0xBA,0xA7,0x80,0x9D,
	    0xEB,0xF6,0xD1,0xCC,0x9F,0x82,0xA5,0xB8,
	    0x03,0x1E,0x39,0x24,0x77,0x6A,0x4D,0x50,
	    0xA1,0xBC,0x9B,0x86,0xD5,0xC8,0xEF,0xF2,
	    0x49,0x54,0x73,0x6E,0x3D,0x20,0x07,0x1A,
	    0x6C,0x71,0x56,0x4B,0x18,0x05,0x22,0x3F,
	    0x84,0x99,0xBE,0xA3,0xF0,0xED,0xCA,0xD7,
	    0x35,0x28,0x0F,0x12,0x41,0x5C,0x7B,0x66,
	    0xDD,0xC0,0xE7,0xFA,0xA9,0xB4,0x93,0x8E,
	    0xF8,0xE5,0xC2,0xDF,0x8C,0x91,0xB6,0xAB,
	    0x10,0x0D,0x2A,0x37,0x64,0x79,0x5E,0x43,
	    0xB2,0xAF,0x88,0x95,0xC6,0xDB,0xFC,0xE1,
	    0x5A,0x47,0x60,0x7D,0x2E,0x33,0x14,0x09,
	    0x7F,0x62,0x45,0x58,0x0B,0x16,0x31,150,
	   // 0x97,0x8A,0xAD,0xB0,0xE3,0xFE,0xD9,0xC4,

	};
 #endif
void time_dsp(void);
//---------------初使化-----------------
void init(void)

{ 

 G_S_IO_SET;                                     
 HIGH_LOWS_PEED_IO_SET;                
 FRONT_BACK_IO_SET ;                   
 POWER_SET;                  
 PROT_SET;                   
 RELY_SET;                    
 BACK_CTRL_SET;  					   
 FRONT_CTRL_SET;
 PWMOUT_SET;	   
 P13_PushPull_Mode;
	
 		PWM5_P15_OUTPUT_ENABLE;	
		PWM_IMDEPENDENT_MODE;
		PWM_CLOCK_DIV_16;//PWM_CLOCK_DIV_8;
		PWMPH = 0x00;
		PWMPL = 0xFF;//F0=4.1K  FF=3.9K
/**********************************************************************
	PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
								= (16MHz/8)/(0x7CF + 1)
								= 1KHz (1ms)
***********************************************************************/	
		set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
		PWM5H = 0x00;						
		PWM5L = 0x00;
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
    Enable_ADC_AIN5;


            clr_ADCF;
            set_ADCS; 
			EADC=0;                                                                                                                                       // Each time ADC start trig signal
     // while(ADCF == 0);
//////////////////////////////
P0=0XFF;
P1=0XDF;

 PWM=0;
CHANGE_Off_TM=0;
PWM_MAX_Set=255;
PWMold=1;
PROT=0;
IO_1=0;
IO_2=0;
CHANGE_FL=1;
A_DET_FL=0;
PWM_Ele_Set=128;
 }



//---------中断--计数器加一--------------
/*void ints (void)//50US 5
{  
  if(1)//FT0IRQ)   //t0中断?
    { 
		//  FT0IRQ=0;
		//  T0C=231;
       //count_500US_CT++;
        //  TC0R=PWM;
    }
} */
 void Timer3_ISR (void) interrupt 16 
{
  //static unsigned char count_500US_PWM ; 
	   clr_TF3;
  //  BACK_CTRL = ~BACK_CTRL;                                 //P0.2 toggle when TF3 interrupt
	//	printf("\n TM3 INT 0x%bX", RH3);
    count_500US_CT++;
  // 	PWM5L = 80;//PWM;
/*	count_500US_PWM++;
	if(Cur_ResultNew==count_500US_PWM)
	{
     PROT=0;	
	}
	if(Cur_ResultNew&&(count_500US_PWM==0))
		 PROT=1;	*/
	
}





 void  MoveCtrl(void)
 {	 
//PWM=80;//
  if(PWMold!=PWM)
  {
    PWMold=PWM;
		set_SFRPAGE;						//PWM4 and PWM5 duty seting is in SFP page 1
		PWM5H = 0x00;						
		PWM5L = PWM;
		clr_SFRPAGE;		
		 set_LOAD;
  //  set_PWMRUN;
  }
 }



void time_dsp(void)
{

	
   if(count_500US_CT)//;count_500US_CT>=10U;count_500US_CT-=10)//50*10=500US
	   {
	     count_500US_CT=0;
 
			 
			

			 
			 
			 
			 
           Count2_5MS++;
        if(Count2_5MS>=8)
		{
		Count2_5MS=0;
	    Count1S++;
     if(Count1S>250)
	 {
        Count1S=0;
       if(CHANGE_Off_TM)
	   {
	      CHANGE_Off_TM--;
		//  PROT=1;
		  }
		  else
		 // PROT=0;

		if(PROT_High_ct)
           PROT_High_ct--;
		
      }
	  }
 ////////////////////////////////////////////////////           
	
/*	  FRONT_NEW_FL=0;
	 BACK_NEW_FL=0;	
     if(CHANGE_Off_TM==0)
      {
/////三态口检测//////////////////////////// 
   if(FRONT_BACK_IO==0)//FP1.2
     BACK_NEW_FL=1;
   else
   {
   P1UR=0XFB;//上拉去掉            
   P1M=0X5;//;强下拉 
FRONT_BACK_IO=0;
FRONT_BACK_IO=0;
FRONT_BACK_IO=0;
   for(temp=0;temp<10;temp++);
    P1M=0X1;//设为输入
  for(temp=0;temp<10;temp++);
   if(FRONT_BACK_IO)////接到高电平   悬空
    FRONT_NEW_FL=1;
   P1UR=0XFF;//上拉加上         
    }
  }
///////三态口检测结束/////////
  BACK_FRONT_New=0;
  if(FRONT_NEW_FL)
   BACK_FRONT_New=1;
  if(BACK_NEW_FL)
   BACK_FRONT_New+=2;

  if(BACK_FRONT_Old!=BACK_FRONT_New)
  {
   BACK_FRONT_Old=BACK_FRONT_New;
   BACK_FRONT_same=0;
  }
  else
  {
  BACK_FRONT_same++;
if(BACK_FRONT_same>20)
  {
    FRONT_FL=FRONT_NEW_FL;
    BACK_FL=BACK_NEW_FL;
  }
  }
  */

 

/////////////////////////////////////
if(G_S_Old!=G_S_IO)
  {
   G_S_Old=G_S_IO;
   G_S_same=0;
  }
  else
  {
  G_S_same++;
if(G_S_same>100)
  G_S_FL=G_S_Old;
  
  }
/*if(A_DET_Old!=A_DET_IO)
  {
   A_DET_Old=A_DET_IO;
   A_DET_same=0;
  }
  else
  {
  A_DET_same++;
if(A_DET_same>200)
  A_DET_FL=A_DET_Old;
  
  }*/
 if((A_DET_IO==0)||ELE30_BIG_ERR_FL)
 {
	ELE_ERR_FL=1;
	TurnHighSpeed_FL=0;
	TurnHighSpeed_FL_New=0;
 }
	else
 if(G_S_FL==0)
 {
	 	ELE_ERR_FL=0;
	 TurnHighSpeed_FL=0;
	TurnHighSpeed_FL_New=0;
 }
 
if(FRONT_BACK_Old!=FRONT_BACK_IO)
  {
   FRONT_BACK_Old=FRONT_BACK_IO;
   FRONT_BACK_same=0;
  }
  else
  {
  FRONT_BACK_same++;
if(FRONT_BACK_same>20)
  FRONT_BACK_FL=FRONT_BACK_Old;
  
  }


if((CHANGE_Off_TM==0)&&G_S_FL&&(ELE_ERR_FL==0))
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
    FRONT_FL=0;
    BACK_FL=0;

   }


if(HIGH_LOWS_PEED!=HIGH_LOWS_PEED_IO)
  {
   HIGH_LOWS_PEED=HIGH_LOWS_PEED_IO;
   HIGH_LOW_same=0;
  }
  else
  {
  HIGH_LOW_same++;
if(HIGH_LOW_same>20)
  HIGH_LOWS_PEED_FL=HIGH_LOWS_PEED;
  
  }


if(CHANGE!=CHANGE_IO)
  {
   CHANGE=CHANGE_IO;
   CHANGE_same=0;
  }
  else
  {
  CHANGE_same++;
if(CHANGE_same>200)
 CHANGE_FL=CHANGE;
  
  }

/*if(CHANGE_FL==0)///||A_DET_FL)
  {
   CHANGE_Off_TM=60;
   //PROT_High_ct=60;
   
   }
*/
	
	P13=HIGH_LOWS_PEED_FL;
	if(HIGH_LOWS_PEED_FLold!=HIGH_LOWS_PEED_FL)
	{
		HIGH_LOWS_PEED_FLold=HIGH_LOWS_PEED_FL;
		TurnHighSpeed_FL=0;
		TurnHighSpeed_FL_New=0;
		ELE_BIG_TM=0;
	}
	
if(HIGH_LOWS_PEED_FL)//||TurnHighSpeed_FL)
PWM_MAX_Set=248;
else
{
PWM_MAX_Set=PWM_Ele_Set;//128;
	if(PWM>PWM_MAX_Set)
		PWM=PWM_MAX_Set;
}
///////////////////////////////////////
	
      if(FRONT_FL)
        {

         if(FRONT_CTRL)
        DelayOffTM=25;//50MS/20MS=3
          BeginMoveBackTM=0;
		if(BeginMoveFrontTM<250)
		  BeginMoveFrontTM++;
	  
		 if(BackOFFTime>24)
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
		 DelayOffTM=25;
		 BeginMoveFrontTM=0;
		 if(BeginMoveBackTM<250)
		  BeginMoveBackTM++;
		if(FrontOFFTime>24) 
         {
        FRONT_CTRL=0;
		BackOFFTime=0;
		 BACK_CTRL=1;
		  RELY=1;
		 }
		 else
        BeginMoveBackTM=0;

		 }
		 else
		 {
         BeginMoveBackTM=0;
		 BeginMoveFrontTM=0;
		 }
      
		}
   if((BeginMoveBackTM>100)||(BeginMoveFrontTM>100))            //50ms/0.5MS=100
      {

              if(PWM<51)
                PWM=51;//起动

	         PWMADDTM++;
		if(PWMADDTM>50)//30 50 70
		{
			PWMADDTM=0;
	 

		if((PWM>PWM_MAX_Set)&&(PWM>51))
				PWM--;
        
		if(PWM<PWM_MAX_Set)
				PWM++;
		  	
      	 }
					 	  		  	    	 

	   }
	   else
      {
	    PWM=0;
       }	
	
	      
			
			TIME_20MS_CT++;
		  if(TIME_20MS_CT>=41U)//20MS 错开来
		  {
            TIME_20MS_CT=0;


				 if(ELE50_BIG_FL)
			 {
				 if(++ELE50_BIG_TM>25)//500/20=25						 
         {
	         ELE30_BIG_FL=1;
	         ELE30_BIG_ERR_FL=1;
					 ELE50_BIG_TM=0;
          }
			 }
			 else
			 {
				 ELE50_BIG_TM=0;
			 }

       if(++time_500ms>25)//0.5s   
			 {
				 time_500ms=0;
				 
		
				 
				 
				 
			   AD_Max=ELE_5S[1];
			 	 AD_Min=ELE_5S[1];
				for(temp=0;temp<(sizeof(ELE_5S)-1);temp++)
				{
					ELE_5S[temp]=ELE_5S[temp+1];
					
          if(AD_Max<ELE_5S[temp])
					{
						 AD_Max=ELE_5S[temp];
						 AD_Max_Index=temp;
					}
					
				 if(AD_Min>ELE_5S[temp])
				 {
						AD_Min=ELE_5S[temp];
					  AD_Min_Index=temp;
				 }
				}
				
				ELE_5S[(sizeof(ELE_5S)-1)]=Cur_ResultNew;
				
				  if(AD_Max<Cur_ResultNew)
					{
						 AD_Max=Cur_ResultNew;		
					 	AD_Max_Index=(sizeof(ELE_5S)-1);
					}						
				 if(AD_Min>Cur_ResultNew)
				 {
						AD_Min=Cur_ResultNew;
					  AD_Min_Index=(sizeof(ELE_5S)-1);
				 }
				
				
				/*if(TurnHighSpeed_FL)
				{
					temp=AD_Max-AD_Min;
					if(((temp>ELE_2A)&&(AD_Min_Index>AD_Max_Index))||(Cur_ResultNew<=ELE_2A))
						TurnHighSpeed_FL=0;
					
				}*/
				 
				 /*计算理论电流*/
				 
				 if(HIGH_LOWS_PEED_FL==0)/*低速才进入调整*/
				 {
					 
				 if(PWM>128)/*计算一下PWM对应的电流*/
				 {
					temp= PWM-128;
				  ELE_set0=temp*86/120; 
					ELE_set0+=ELE11A; 
				 }
				 else
					 ELE_set0=Cur_ResultNew;//0;


					 temp=0;
			if(TurnHighSpeed_FL_New!=TurnHighSpeed_FL)
			  {
			   if((TurnHighSpeed_FL_New==1)&&(TurnHighSpeed_FL==0))
			    temp=1;
				TurnHighSpeed_FL=TurnHighSpeed_FL_New;
			  }
					
				 if(temp)
				 {
						if(Cur_ResultNew>ELE11A)
						{
						  PWM_Ele_Set=128;									
						 for(temp=Cur_ResultNew-ELE11A;temp>=ELE_1A;temp-=ELE_1A)
						 {
                if(PWM_Ele_Set<=(248-7))
						    PWM_Ele_Set+=7;
								else
								{
								 PWM_Ele_Set=248;	
									break;
								}							 							 
													
               }																
						}
						else
					{
				    PWM_Ele_Set=128; 
					//	TurnHighSpeed_FL=0;
					//	TurnHighSpeed_FL_New=1;
					}
						
					  PWM=PWM_Ele_Set;		
				 }
				 else				 
					if(TurnHighSpeed_FL)	//ELE_1A
					{						
        if(Cur_ResultNew>ELE_set0)
				{
          temp=Cur_ResultNew-ELE_set0;
					if(temp>ELE_2A)
					{/*电流大于2A以上*/
						// PWM_Ele_Set=128;				   								
						 for(;temp>=ELE_1A;temp-=ELE_1A)
						 {
                if(PWM_Ele_Set<=(248-7))
						    PWM_Ele_Set+=7;//13;
								else
								{
								 PWM_Ele_Set=248;	
									break;
								}							 							 
													
               }							
						  PWM=PWM_Ele_Set;		
					}
					
				}
				else
				{
					 temp=ELE_set0-Cur_ResultNew;

	        if(temp>ELE_2A)
					{
						
					  // PWM_Ele_Set=128;									
						 for(;temp>=ELE_1A;temp-=ELE_1A)
						 {
                if(PWM_Ele_Set>=(128+7))
						    PWM_Ele_Set-=7;//13;
								else
								{
								 PWM_Ele_Set=128;
								 	TurnHighSpeed_FL=0;
					          	TurnHighSpeed_FL_New=1;	
									break;
								}							 							 
													
                          }							
						  PWM=PWM_Ele_Set;		
					}
				}	
				
				  if(Cur_ResultNew<ELE11A)
				  {
				  TurnHighSpeed_FL=0;
    	          TurnHighSpeed_FL_New=0;
				  }
				  			
					}
					else	 
					{
					 PWM_Ele_Set=128;//;					
					}				
				}	
				
		
			if(TurnHighSpeed_FL)
				PROT=!PROT;
			else
				PROT=0;
					
					if(ELE30_BIG_FL)
				{
					
				if(++ELE30_BIG_TM>4)//10S==10000/20=500
				  {
						ELE30_BIG_ERR_FL=1;//
						ELE30_BIG_TM=0;
					}
				}
				else
				{
					ELE30_BIG_TM=0;
           ELE30_BIG_ERR_FL=0;					
				}
				
				if(ELE13_BIG_FL)
				{
					
				if(++ELE13_BIG_TM>2)//10S==10000/20=500
				  {
						TurnHighSpeed_FL_New=1;//TurnHighSpeed_FL=1;
						ELE13_BIG_TM=0;
					}
				}
				else
					ELE13_BIG_TM=0;	
				
				if(ELE_BIG_FL)
				{					
				if(++ELE_BIG_TM>6)//10S==10000/20=500
				  {
						TurnHighSpeed_FL_New=1;//TurnHighSpeed_FL=1;
						ELE_BIG_TM=0;
					}
				}
				else
					ELE_BIG_TM=0;					
			}
				
if(G_S_FL)
{
 POWER=1;
G_S_FL_CT=0;
 }
 else
 {
 if(++G_S_FL_CT1>50)
 {
   G_S_FL_CT1=0;
   if(++G_S_FL_CT>=1800)
  {
     G_S_FL_CT=1800;
	  POWER=0;
	}
 }


 }
  if(FRONT_CTRL==0)
	{
	if(FrontOFFTime<250) 
       FrontOFFTime++; //关断时间长度
     }
	 else
      FrontOFFTime=0;

  if(BACK_CTRL==0)
	{
	if(BackOFFTime<250) 
        BackOFFTime++;//关断时间长度
     }
	 else
      BackOFFTime=0;
/////////////////////////////////////////////////
             if(DelayOffTM)
               DelayOffTM--;
			   else
			   {
			   	RELY=0;
                FRONT_CTRL=0;
                BACK_CTRL=0;		
				BeginMoveBackTM=0;
				BeginMoveFrontTM=0;//完全停下来才清零
			   }

          

   }		
		

	   } 
		 
 
}

void ADC_DSP(void)
{
unsigned char ADtemp;
 ADCF=0;
 ADtemp=ADCRH;
	
     Hall_Add+=ADtemp;            
	   Hall_Add_ct++;
	   if(Hall_Add_ct>=128)	//??32?,???? 64
	   {
	   Hall_Add_ct=0;
	   Cur_ResultNew=Hall_Add>>7;//??
	   Hall_Add=0;	


if(Cur_ResultNew>ELE50A)
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

if(Cur_ResultNew>ELE30A)
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
	if(Cur_ResultNew>ELE13A)
  {
		
  ELE13_BIG_ct++;
	ELE13_Small_ct=0;
  if(ELE13_BIG_ct>5)
		ELE13_BIG_FL=1;
  }
  else
	{
		
	  ELE13_BIG_ct=0;
		ELE13_Small_ct++;			
		if(ELE13_Small_ct>5)
			ELE13_BIG_FL=0;
	}
	
  if(Cur_ResultNew>ELE11A)
  {
		
  ELE_BIG_ct++;
	ELE_Small_ct=0;
  if(ELE_BIG_ct>5)
		ELE_BIG_FL=1;
  }
  else
	{
		
	  ELE_BIG_ct=0;
		ELE_Small_ct++;			
		if(ELE_Small_ct>5)
			ELE_BIG_FL=0;
	}
	
  }

 set_ADCS; 
}
//-----------------主程序-----------------------------
void main(void)
{ 
  init( );
 while(1)
  {  
                                      //Timer3 ru
  time_dsp();  
  MoveCtrl();
 if(ADCF)
	ADC_DSP();
 
  }
}
