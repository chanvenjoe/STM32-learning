/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USB_DET_Pin GPIO_PIN_13
#define USB_DET_GPIO_Port GPIOC
#define CHG_STA_Pin GPIO_PIN_14
#define CHG_STA_GPIO_Port GPIOC
#define AIR_PRES_CSD_Pin GPIO_PIN_0
#define AIR_PRES_CSD_GPIO_Port GPIOH
#define BT_STA_Pin GPIO_PIN_0
#define BT_STA_GPIO_Port GPIOC
#define BT_WAKE_Pin GPIO_PIN_1
#define BT_WAKE_GPIO_Port GPIOC
#define SYS_WAKE_Pin GPIO_PIN_2
#define SYS_WAKE_GPIO_Port GPIOC
#define AD_BAT_Pin GPIO_PIN_3
#define AD_BAT_GPIO_Port GPIOA
#define Win1_Pin GPIO_PIN_6
#define Win1_GPIO_Port GPIOC
#define Win2_Pin GPIO_PIN_7
#define Win2_GPIO_Port GPIOC
#define Win3_Pin GPIO_PIN_8
#define Win3_GPIO_Port GPIOC
#define Win4_Pin GPIO_PIN_9
#define Win4_GPIO_Port GPIOC
#define WS_LED_Pin GPIO_PIN_12
#define WS_LED_GPIO_Port GPIOA
#define BKL_Pin GPIO_PIN_15
#define BKL_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_10
#define CS_GPIO_Port GPIOC
#define D_C_Pin GPIO_PIN_11
#define D_C_GPIO_Port GPIOC
#define LCD_RES_Pin GPIO_PIN_12
#define LCD_RES_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
