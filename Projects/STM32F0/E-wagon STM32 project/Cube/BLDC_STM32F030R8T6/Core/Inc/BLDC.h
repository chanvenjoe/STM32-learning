/*
 * BLDC.h
 *
 *  Created on: Aug 8, 2023
 *      Author: kzhou
 */

#ifndef BLDC_H_
#define BLDC_H_

#include "main.h"

#define MOSL GPIOB
#define	MOSH GPIOA
#define AH	 TIM_CHANNEL_1
#define BH	 TIM_CHANNEL_2
#define CH	 TIM_CHANNEL_3
#define AL	 GPIO_PIN_13
#define BL	 GPIO_PIN_14
#define CL	 GPIO_PIN_15
//Close all MOS-> open target mos
#define CLOSE_ALL 	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1); HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2); HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3); //HAL_GPIO_WritePin(MOSL, AL|BL|CL, GPIO_PIN_RESET)
#define AHBL_ON		CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, AH); //HAL_GPIO_WritePin(MOSL, BL, GPIO_PIN_SET)
#define	AHCL_ON	  	CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, AH); //HAL_GPIO_WritePin(MOSL, CL, GPIO_PIN_SET)
#define	BHAL_ON		CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, BH); //HAL_GPIO_WritePin(MOSL, AL, GPIO_PIN_SET)
#define	BHCL_ON		CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, BH); //HAL_GPIO_WritePin(MOSL, CL, GPIO_PIN_SET)
#define CHAL_ON		CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, CH); //HAL_GPIO_WritePin(MOSL, AL, GPIO_PIN_SET)
#define CHBL_ON		CLOSE_ALL; HAL_TIM_PWM_Start(&htim1, CH); //HAL_GPIO_WritePin(MOSL, BL, GPIO_PIN_SET)

#define BRAKE		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);  HAL_GPIO_WritePin(MOSL, AL, GPIO_PIN_SET); //HAL_GPIO_WritePin(MOSL, BL, GPIO_PIN_SET)
#define BRKRLS		HAL_GPIO_WritePin(MOSL, BL, GPIO_PIN_RESET);
#define CLOSE_PWM 	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2)//; HAL_GPIO_WritePin(MOSL, AL|BL|CL, GPIO_PIN_RESET)
#define BHALONBLOFF	//HAL_GPIO_WritePin(MOSL, BL, GPIO_PIN_RESET); HAL_GPIO_WritePin(MOSL, AL, GPIO_PIN_SET); HAL_TIM_PWM_Start(&htim1, BH)

void BLDC_Start_Up();
//void BLDC_Phase_switching(MADC_Structure * adc_val);

#endif /* BLDC_H_ */

/////bldc equations////
/*

 1: BEMF = NlrBW
 number of whinding turns per phase
 the length of the rotor
 the internal radius of the rotor
 the rotor magnetic field density
 the motor angular velocity

 2:	Timer time = 60/RPM * Pole pairs *6
*/
