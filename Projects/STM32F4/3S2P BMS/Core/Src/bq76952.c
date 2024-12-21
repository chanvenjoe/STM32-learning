/*
 * bq76952.c
 *
 *  Created on: Nov 24, 2024
 *      Author: kzhou
 */

#include "bq76952.h"
#include "i2c.h"
#include "main.h"


void read_cell_1()
{
	uint16_t device_addr = 0x68;
	uint8_t reg_addr = 0x66;
	uint8_t read_buff[3] = {0x82, 0xF0};
	HAL_StatusTypeDef hal_status  = HAL_I2C_Master_Transmit(&hi2c1, device_addr<<1, &reg_addr, 1, 1000);

//	HAL_I2C_Master_Receive(&hi2c1, device_addr, read_buff, 2, HAL_MAX_DELAY);
}



