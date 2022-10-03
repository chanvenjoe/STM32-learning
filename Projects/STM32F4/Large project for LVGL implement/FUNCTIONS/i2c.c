#include "i2c.h"
#include "delay.h"

void MyI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_SCL = 1;
	I2C_SDA = 1;
}

void I2C_Start()
{
	SDA_OUT();
	I2C_SCL = 1;
	I2C_SDA = 1;
	delay_us(4);
	I2C_SCL = 0;
}

void I2C_Stop()
{
	SDA_OUT();
	I2C_SCL = 0;
	I2C_SDA	= 0;
	delay_us(4);
	I2C_SCL = 1;
	delay_us(4);
	I2C_SCL = 1;
	I2C_SDA = 1;
	delay_us(4);
}