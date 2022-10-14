
#include "spi.h"

void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef	SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_SPI1);
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
	
	SPI_InitStructure.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_CPHA				=	SPI_CPHA_2Edge; //The CPHA AND CPOLE should follow the slave device
	SPI_InitStructure.SPI_CPOL				=	SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial		=	7;				//CRC value 
	SPI_InitStructure.SPI_DataSize			=	SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction			=	SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit			=	SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode				=	SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS				=	SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	SPI1_ReadWriteByte(0XFF); //Make the MOSI line high
	
}

//Send a byte and return a byte
u8 SPI1_ReadWriteByte(u8 TxDat)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET){}		// wait for the send buffer empty
	SPI_I2S_SendData(SPI1,TxDat);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET){}
	return SPI_I2S_ReceiveData(SPI1);

}

void SPI1_Set_BaudRate(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0xffc7;
	SPI1->CR1|= SPI_BaudRatePrescaler;
	SPI_Cmd(SPI1,ENABLE);
}

