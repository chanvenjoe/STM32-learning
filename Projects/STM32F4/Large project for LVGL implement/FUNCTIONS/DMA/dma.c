#include "dma.h"
#include "sys.h"

//par: peripheral addr
//mar: memory addr
//ndtr: number of data transmitted
void MyDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	if((u32)DMA_Streamx>(u32)DMA2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	}
	else
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	}
	DMA_DeInit(DMA_Streamx);
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);
	
	DMA_InitStructure.DMA_BufferSize 	= ndtr;
	DMA_InitStructure.DMA_Channel		= chx;
	DMA_InitStructure.DMA_DIR			= DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_FIFOMode		= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;
	DMA_InitStructure.DMA_MemoryBurst	= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_MemoryDataSize= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc		= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode			= DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc	= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority		= DMA_Priority_Medium;
	DMA_Init(DMA_Streamx, &DMA_InitStructure);

	
}

void My_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);
	while(DMA_GetCmdStatus(DMA_Streamx)!=DISABLE);
	DMA_SetCurrDataCounter(DMA_Streamx, ndtr);
	DMA_Cmd(DMA_Streamx, ENABLE);
}