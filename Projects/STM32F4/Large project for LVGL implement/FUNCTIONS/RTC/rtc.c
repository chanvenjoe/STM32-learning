#ifndef 	__RTC_H
#define 	__RTC_H

#include "rtc.h"
#include "sys.h"
u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0x1fff;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!0X5050)
	{
		RCC_LSEConfig(RCC_LSE_ON);

#endif