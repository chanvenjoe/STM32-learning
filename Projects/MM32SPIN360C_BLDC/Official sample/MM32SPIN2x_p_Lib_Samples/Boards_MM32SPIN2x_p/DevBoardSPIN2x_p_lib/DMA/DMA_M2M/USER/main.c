/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.0
* @date     09/09/2019
* @brief
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/
#include "HAL_device.h"
#include "HAL_conf.h"

unsigned char dma1Flag = 0x0;
unsigned char dma2Flag = 0x0;
u8  dmaTxDATA[64] ;
u8  dmaRxDATA[64] ;

char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendByte(u8 dat);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);

void delay_us(__IO uint32_t nTime);
void delay_ms(__IO uint32_t nTime);
void LED_Init(void);
void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);
void DMAcheckStatus(uint32_t DMA_FLAG);
void DMA_m8tom8_test(void);
void DMA_m8tom16_test(void);
void DMA_m8tom32_test(void);
void DMA_m16tom8_test(void);
void DMA_m16tom16_test(void);
void DMA_m16tom32_test(void);

#define LED2_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define LED2_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_8))?(GPIO_ResetBits(GPIOA,GPIO_Pin_8)):(GPIO_SetBits(GPIOA,GPIO_Pin_8))

#define LED1_ON()  GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED1_OFF()  GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))?(GPIO_ResetBits(GPIOC,GPIO_Pin_13)):(GPIO_SetBits(GPIOC,GPIO_Pin_13))

/*******************************************************************************
* @name   : main
* @brief  : DMA
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    LED_Init();
    Uart_ConfigInit(9600);
//    DMA_m8tom8_test();
//    DMA_m8tom16_test();
//    DMA_m8tom32_test();
    DMA_m16tom8_test();
//    DMA_m16tom16_test();
//    DMA_m16tom32_test();

    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        delay_ms(1000);
    }
}

/*******************************************************************************
* @name   : DMA1_Channel1_IRQHandler
* @brief  : DMA1 channel 1 interrupt function
* @param  : None
* @retval : void
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        dma1Flag = 0x1;
    }
}

/*******************************************************************************
* @name   : DMA1_Channel2_3_IRQHandler
* @brief  : DMA1 channel 2_3 interrupt function
* @param  : None
* @retval : void
*******************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        dma2Flag = 0x1;
    }

}

/*******************************************************************************
* @name   : LED_Init
* @brief  : Config LED
* @param  : None
* @retval : void
*******************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    LED1_OFF();
    LED2_OFF();
}

/*******************************************************************************
* @name   : Uart_ConfigInit
* @brief  : Uart Config Init
* @param  : u32 bound
* @retval : void
*******************************************************************************/
void Uart_ConfigInit(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    //Serial baud rate
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    //Transceiver mode
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //Multiplexing push-pull output
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //UART1_RX	  GPIOA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    //Floating input
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : UartSendByte
* @brief  : Uart Send Byte
* @param  : u8 dat
* @retval : void
*******************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

/*******************************************************************************
* @name   : UartSendGroup
* @brief  : Uart Send Group
* @param  : u8* buf,u16 len
* @retval : void
*******************************************************************************/
void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

/*******************************************************************************
* @name   : UartSendAscii
* @brief  : Uart Send Ascii
* @param  : char *str
* @retval : void
*******************************************************************************/
void UartSendAscii(char *str)
{
    while(*str)
        UartSendByte(*str++);
}

/*******************************************************************************
* @name   : delay_us
* @brief  : delay us
* @param  : nTime
* @retval : void
*******************************************************************************/
void delay_us(__IO uint32_t nTime)
{
    u16 i = 0;
    while(nTime--)
    {
        i = 10;
        while(i--);
    }
}

/*******************************************************************************
* @name   : delay_ms
* @brief  : delay ms
* @param  : nTime
* @retval : void
*******************************************************************************/
void delay_ms(__IO uint32_t nTime)
{
    u16 i = 0;
    while(nTime--)
    {
        i = 750;
        while(i--);
    }
}

/*******************************************************************************
* @name   : DMAcheckStatus
* @brief  : Query DMA flag
* @param  : DMA_FLAG
* @retval : void
*******************************************************************************/
void DMAcheckStatus(uint32_t DMA_FLAG)
{
    while(1)
    {
        if(DMA_GetFlagStatus(DMA_FLAG))
        {
            DMA_ClearFlag(DMA_FLAG);
            break;
        }
    }
}

/*******************************************************************************
* @name   : DMA_m8tom8_test
* @brief  : Move 8-bit data to another address and stored in 8-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m8tom8_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

    dma2Flag = 0x0;

    DMA_Cmd(DMA1_Channel2, ENABLE);

    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }


    DMA_Cmd(DMA1_Channel2, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = dmaRxDATA[i];

        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}

/*******************************************************************************
* @name   : DMA_m8tom16_test
* @brief  : Move 8-bit data to another address and stored in 16-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m8tom16_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel1);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    dma1Flag = 0x0;

    DMA_Cmd(DMA1_Channel1, ENABLE);

    while(1)
    {
        if(dma1Flag)
        {
            dma1Flag = 0x0;
            break;
        }
    }

    DMA_Cmd(DMA1_Channel1, DISABLE);
    for(i = 0; i < 128; i++)
    {

        temp = dmaRxDATA[i];

        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}

/*******************************************************************************
* @name   : DMA_m8tom32_test
* @brief  : Move 8-bit data to another address and stored in 32-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m8tom32_test()
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel3);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC3);

    DMA_Cmd(DMA1_Channel3, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = dmaRxDATA[i];
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}

/*******************************************************************************
* @name   : DMA_m16tom8_test
* @brief  : Move 16-bit data to another address and stored in 8-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m16tom8_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel4);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);


    DMA_Cmd(DMA1_Channel4, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC4);

    DMA_Cmd(DMA1_Channel4, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = dmaRxDATA[i];

        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}

/*******************************************************************************
* @name   : DMA_m16tom16_test
* @brief  : Move 16-bit data to another address and stored in 16-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m16tom16_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel5);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);


    DMA_Cmd(DMA1_Channel5, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC5);

    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = dmaRxDATA[i];

        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}

/*******************************************************************************
* @name   : DMA_m16tom32_test
* @brief  : Move 16-bit data to another address and stored in 32-bit mode
* @param  : None
* @retval : void
*******************************************************************************/
void DMA_m16tom32_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;

    p = (u8*)dmaTxDATA;
    for(i = 0; i < 64; i++)
    {
        *(p + i) = i + 1;
    }

    DMA_DeInit(DMA1_Channel5);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel5, ENABLE);

    DMAcheckStatus(DMA1_FLAG_TC5);

    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i = 0; i < 64; i++)
    {

        temp = dmaRxDATA[i];

        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp%d=0x%x\r\n", i, temp));
    }
}


/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

