#include "stbwaup.h"
#include "sys.h"
#include "delay.h"
void Sys_Enter_Standby(void)
{		
  while(WKUP_KD);			//�ȴ�WK_UP�����ɿ�(����RTC�ж�ʱ,�����WK_UP�ɿ��ٽ������)
 
	RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//��λ����IO��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��		 
	 
	PWR_BackupAccessCmd(ENABLE);//���������ʹ��

	//�������Ǿ�ֱ�ӹر����RTC�ж�
	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//�ر�RTC����жϣ�������RTCʵ����ˡ�
	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//���RTC����жϱ�־λ��
	
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	
	PWR_WakeUpPinCmd(ENABLE);//����WKUP���ڻ���
	
	PWR_EnterSTANDBYMode();	//�������ģʽ
	 
}

u8 Check_WKUP(void) 
{
	u8 t=0;
	u8 tx=0;//��¼�ɿ��Ĵ���
	PFout(9)=0; //����DS0 
	while(1)
	{
		if(WKUP_KD)//�Ѿ�������
		{
			t++;
			tx=0;
		}else 
		{
			tx++; 
			if(tx>3)//����90ms��û��WKUP�ź�
			{
				PFout(9)=1;
				return 0;//����İ���,���´�������
			}
		}
		delay_ms(30);
		if(t>=100)//���³���3����
		{
			PFout(9)=0;	  //����DS0 
			return 1; //����3s������
		}
	}
} 

void LPM_Init()
{
	GPIO_InitTypeDef STB_Key;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	PWR_WakeUpPinCmd(ENABLE);
	PWR_EnterSTANDBYMode();
	
	STB_Key.GPIO_Mode	= GPIO_Mode_IN;
	STB_Key.GPIO_OType	= GPIO_OType_OD;
	STB_Key.GPIO_Pin	= GPIO_Pin_0; 
	STB_Key.GPIO_PuPd	= GPIO_PuPd_DOWN;
	STB_Key.GPIO_Speed	= GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA, &STB_Key);
	
		//(����Ƿ���������)��    		 
	if(Check_WKUP()==0)
	{
		Sys_Enter_Standby();	//���ǿ���,�������ģʽ  
	}
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0
	
 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����NVIC

	
	
	
}	

void EXTI0_IRQHandler(void)
{ 		    		    				     		    
	EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ
	if(Check_WKUP())//�ػ�?
	{		  
		Sys_Enter_Standby(); //�������ģʽ 
	}
} 
