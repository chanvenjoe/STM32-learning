/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "adc.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define TURE  1
#define FALSE 0
#define RX_BUF_NUM 255
#define TX_BUF_NUM 255
extern uint8_t txbuf[TX_BUF_NUM];
extern uint8_t rxbuf[RX_BUF_NUM];
extern uint8_t rxdata;
extern uint8_t cnt;

#define MotorOn		"0X13MOne"//\r\n"
#define MotorOff	"0X13MOffe"//\r\n"
#define PID			"0X13 0XPIDe"


typedef struct{
	int TimeCNT;
	char 	Voltage_Show;
	char	PID_Set;
	char	Motor_On;
	char	Motor_Off;
	char		Motor_short_flag;
	char	OCP_flag;
	char	sensor_err;
}TimeFlagStruct;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void BT_PWM_handle(char flag);
void Print_Pooling(TimeFlagStruct* print_flag);
void Uart_RX_Handle(UART_HandleTypeDef *huart);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

