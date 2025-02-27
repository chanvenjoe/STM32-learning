/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim6;

extern TIM_HandleTypeDef htim14;

extern TIM_HandleTypeDef htim15;

extern TIM_HandleTypeDef htim16;

extern TIM_HandleTypeDef htim17;

/* USER CODE BEGIN Private defines */
//#define Ramp
#define 	TIMERTESTER	weight_par.eps_flag = ! weight_par.eps_flag;\
if(weight_par.eps_flag==0)\
{\
	__HAL_TIM_SET_COUNTER(&htim16, 0);/*the auto reload is set to 65535 1us time base*/\
	HAL_TIM_Base_Start(&htim16);\
}\
else\
{\
	HAL_TIM_Base_Stop(&htim16);\
	adc_val.commutation_delay = __HAL_TIM_GET_COUNTER(&htim16);\
}
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM3_Init(void);
void MX_TIM6_Init(void);
void MX_TIM14_Init(void);
void MX_TIM15_Init(void);
void MX_TIM16_Init(void);
void MX_TIM17_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */
void delay_us(uint16_t us);
void delay_ms(uint32_t delay);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

