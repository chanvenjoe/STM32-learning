#include "rtc.h"
//#include "stm32f4xx_exti.h"

u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	u16 retry=0x1fff;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0X5050)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET)
		{
			retry++;
			delay_ms(10);
		}
		if(retry==0)
			return 1;
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_InitStructure.RTC_AsynchPrediv	= 0x7f; //1-0x7f
		RTC_InitStructure.RTC_SynchPrediv	= 0XFF; //0-7FFF
		RTC_InitStructure.RTC_HourFormat	= RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		
		RTC_TimeStructure.RTC_H12	= RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours	= 0;
		RTC_TimeStructure.RTC_Minutes = 0;
		RTC_TimeStructure.RTC_Seconds = 0;
		RTC_TimeStructInit(&RTC_TimeStructure);
		RTC_WriteBackupRegister(RTC_BKP_DR0,0X5050);
	}
	return 0;
}

void RTC_Set_AlarmA(u8 week, u8 hour, u8 min, u8 sec)
{
	EXTI_InitTypeDef EXTI_Init_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef  RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;
	RTC_AlarmTypeInitStructure.RTC_AlarmTime= RTC_TimeTypeInitStructure;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	EXTI_Init_InitStructure.EXTI_Line=EXTI_Line17;
	EXTI_Init_InitStructure.EXTI_Mode=EX;
	
}

