/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc;

/* USER CODE BEGIN Private defines */
typedef struct {
	uint8_t bemf_pa;
	uint8_t bemf_pb;
	uint8_t bemf_pc;
	uint16_t bemf_mid;
	uint16_t ia;
	uint16_t ib;
	uint16_t isum;
	uint16_t isum_filtered;
	uint16_t vbat;
	uint16_t vref_data;
	uint8_t  	bemf_now;
	uint8_t 	bemf_last;
	uint8_t		bemf_next;
	uint8_t 	zero_across_flag;
	uint16_t	zero_across_count;
	uint16_t	commutation_delay;
	uint16_t	commutation_timeout; //Move to BLDC structure later
	uint16_t	zero_across_threshole;
	uint16_t	speed;
}MADC_Structure;




enum ADC_SEQ{pa=0,pb,pc,ia,ib,isum,isum_filt,vb};

enum BLDC_STATUS
{
	START_UP = 0,
	BEMF_DETECTION,
	TIMEOUT
};

#define CH_NUM 10
#define Vrefint 	*(__IO uint16_t *)(0x1FFFF7BA) * 3.3/4095
#define VBAT_FACTOR	0.126
#define ZERO_ACROSS_THR	20
#define MAJORITY_CONST	6
/* USER CODE END Private defines */

void MX_ADC_Init(void);

/* USER CODE BEGIN Prototypes */
MADC_Structure My_ADC_getvalue(uint16_t * adc_buf, MADC_Structure *adc_val);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

