

#ifndef InitMCU
#define	InitMCU

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_Define.h"
#include "Common.h"
#include "Define.h"

bit BIT_TMP;

extern void MCU_Init(void);
static void GPIO_Init(void);
static void Timer_Init(void);
static void PWM_DEAD_TIME_VALUE(void);
static void PWM_Init(void);
static void ADC_Init(void);
extern SysClock_TypeDef tSysClock;



#endif
