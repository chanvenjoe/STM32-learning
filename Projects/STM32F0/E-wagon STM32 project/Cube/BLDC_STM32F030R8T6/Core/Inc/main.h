/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "BLDC.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void printf_DMA(const char *format, ...);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTWakeUp_Pin GPIO_PIN_15
#define BTWakeUp_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
#define FORCESAPTIME 			static char cnt = 0;\
	cnt++;\
	if(cnt == 1)\
	{	__HAL_TIM_SET_COUNTER(&htim16, 0);\
		HAL_TIM_Base_Start(&htim16);\
	}\
	if(cnt==2)\
	{	adc_val.commutation_delay = __HAL_TIM_GET_COUNTER(&htim16);\
		cnt = 0;\
	}\

#define TRUE 1
#define FALSE 0

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
