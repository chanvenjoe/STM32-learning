#ifndef _I2C__H
#define _I2C__H

#include "sys.h" 

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

#define I2C_SCL 	PBout(8)
#define I2C_SDA		PBin(9)

#define SDA_READ 	PBin(9)

#define I2C_SCLH	GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define I2C_SCLL	GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define I2C_SDAH	GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define I2C_SDAL	GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#define TCS_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define TCS_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
#define TCS_SDA_READ  PBin(9)

#define TCS_SCL_H     GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define TCS_SCL_L     GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define TCS_SDA_H     GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define TCS_SDA_L     GPIO_ResetBits(GPIOB,GPIO_Pin_9)



void MyI2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
u8 I2C_Wait_Ack(void);
void I2C_ACK(void);
void I2C_NACK(void);
void I2C_Send_Byte(u8 txd);
u8 I2C_Read_Byte(u8 ack);
u8 TCS34725_Ready(void);
void I2C_Write(u8 slaveaddr, u8 *arrdata, u8 bytes, u8 stopbit);
void I2C_Read(u8 slaveaddr, u8 regaddr, u8 bytes, u8 stopbit);

#endif

