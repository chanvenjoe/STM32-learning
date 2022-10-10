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
	
//	while(TCS34725_Ready());
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

u8 I2C_Wait_Ack()
{
	u8 Ack_ErrTim;
	SDA_IN();
	I2C_SCL = 1; delay_us(1);
	I2C_SDA = 1; delay_us(1);
	while(SDA_READ)
	{
		Ack_ErrTim++;
		if(Ack_ErrTim>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SDA = 0;
	return 0;
}

void I2C_ACK()
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 0;
	delay_us(2);
	I2C_SCL = 1;
	delay_us(2);
	I2C_SDA = 1;
	I2C_SCL = 0;
}

void I2C_NACK()
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 1;
	delay_us(2);
	I2C_SCL	= 1;
	delay_us(2);
	I2C_SCL = 0;
}

void I2C_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	I2C_SCL = 0;
	for(t=0;t<8;t++)
	{
		I2C_SDA=(txd&0x80)>>7;			//Send the highest bit first
		txd<<=1;
		delay_us(2);
		I2C_SCL=1;
		delay_us(2);
		I2C_SCL = 0;
		delay_us(2);
	}
}
/* ack=1; send ack*/
u8 I2C_Read_Byte(u8 ack)
{
	u8 i, receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_SCL = 0;delay_us(2);
		I2C_SCL	= 1;
		receive<<=1;
		if(SDA_READ)
			receive++;
		delay_us(1);
	}
	if(!ack)
		I2C_NACK();
	else
		I2C_ACK();
	return receive;
}
		
		
	

