#ifndef _I2C__H
#define _I2C__H

#include "sys.h" 

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

#define I2C_SCL 	PBout(8)
#define I2C_SDA		PBin(9)
#define SDA_READ 	PBin(9)

#endif

