#ifndef __DMA_H
#define __DMA_H
#include "sys.h"

void MyDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr);
void My_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr);
#endif