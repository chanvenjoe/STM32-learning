/*
 * sw_spi.h
 *
 *  Created on: Jan 31, 2024
 *      Author: kzhou
 */

#ifndef INC_SW_SPI_H_
#define INC_SW_SPI_H_

#include "main.h"

#define SW_SPI_PORT 	GPIOF

#define SW_SPI_PIN_CLK 	GPIO_PIN_4
#define SW_SPI_PIN_DAT 	GPIO_PIN_5

#define SW_SPI_PWR_OFF	HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_SET)
#define SW_SPI_PWR_ON	HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_RESET)
#define SW_SPI_DAT_RDY  HAL_GPIO_ReadPin(SW_SPI_PORT, SW_SPI_PIN_DAT)
#define SW_SPI_CLK_H	HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_SET)
#define SW_SPI_CLK_L	HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_RESET)

#define FOUR_CLK		SW_SPI_CLK_H; delay_us(1); SW_SPI_CLK_L; SW_SPI_CLK_H; delay_us(1); SW_SPI_CLK_L; SW_SPI_CLK_H; delay_us(1); SW_SPI_CLK_L; SW_SPI_CLK_H; delay_us(1); SW_SPI_CLK_L
//#define SW_SPI_1_CLK_CYL  HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_SET); delay_us(1); HAL_GPIO_WritePin(SW_SPI_PORT, SW_SPI_PIN_CLK, GPIO_PIN_RESET); delay_us(1)


#define CHA_128	25
#define CHB_32	26
#define CHA_64	27
#define LOAD_CELL_FACTOR 225 //Example: A Kg * 3.24mV/10Kg => A*128 = 0.324*128AmV = 41.472AmV  => 41.472AmV/AD = 3.24V/0xffffff => AD = 214748.352A A g = AD/214.748
									//If the real number is smaller than the tested value, then increase the factor, vice versa
#define PULL_FORCE_THR	1000
#define LOWER_LIMMIT	150

#define CALIBRATION_RANGE_L 8000000
#define CALIBRATION_RANGE_H	8300000


typedef struct {
	 int	gram;
	 int	gross_weight;
	 int	calibrated_value;
	 char	calibration_flag;
	 char	eps_flag;
}HX711_Structure;

unsigned int Get_24bit_Weight(char channel_gain);
void HX711_Calibration( HX711_Structure* weight_par);
void Get_weight(HX711_Structure* weight_par);
void PWM_Delegation(HX711_Structure* weight_par);
char Incremental_PID(HX711_Structure* weight_par, uint16_t pull_force_thr);

#endif /* INC_SW_SPI_H_ */