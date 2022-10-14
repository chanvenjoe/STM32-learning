#ifndef _SPI__H
#define _SPI__H

#include "sys.h"


void SPI1_Init(void);
u8 SPI1_ReadWriteByte(u8 TxDat);
void SPI1_Set_BaudRate(u8 SPI_BaudRatePrescaler);

#endif
