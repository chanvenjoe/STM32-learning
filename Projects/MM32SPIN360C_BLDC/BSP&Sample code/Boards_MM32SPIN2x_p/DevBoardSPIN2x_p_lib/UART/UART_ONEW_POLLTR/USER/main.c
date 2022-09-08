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
#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"

#define TEST_SELE_HALF_DATA_DISABLE     0
#define TEST_SELE_HALF_DATA_ENABLE      1

#define TEST_HALF_DATA                  TEST_SELE_HALF_DATA_ENABLE

char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendByte(u8 dat);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);

void RCC_ConfigInit(void);
void GPIO_ConfigInit(void);
void NVIC_ConfigInit(void);
void initGPIO_UART(void);
void uartConfig(unsigned int bound);

u16 rxdata;
char *pTx = printBuf;

u8 rx_cnt = 0, tx_flag;
u32 SysTick_Count;
u32 gDlycnt;

/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : void
* @retval : void
*******************************************************************************/
void InitSystick()
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

/*******************************************************************************
* @name   : SysTick_Handler
* @brief  : SysTick interrupt handle function
* @param  : void
* @retval : void
*******************************************************************************/
void SysTick_Handler()
{
    if(SysTick_Count++ > 500)
    {
        SysTick_Count = 0;
    }
    if(gDlycnt > 0) gDlycnt --;
}

/*******************************************************************************
* @name   : main
* @brief  : main function
            PB7:TX,PA6:RX;
            BAUD = 9600;
            Interrupt mode
            Function :Send the received data
* @param  : void
* @retval : void
*******************************************************************************/
int main(void)
{
    int i;
    InitSystick();
    RCC_ConfigInit();
    NVIC_ConfigInit();
    Uart_ConfigInit(9600);
    UartSendByte(0x55);

    i = UART_ReceiveData(UART1);
    while(1)
    {

        if(tx_flag != 0)
        {
            UART_ITConfig( UART1, UART_IT_TXIEN, ENABLE);                       //clear send flag;
        }
    }
}

/*******************************************************************************
* @name   : UART1_IRQHandler
* @brief  : UART1 interrupt handler
* @param  : void
* @retval : void
*******************************************************************************/
void UART1_IRQHandler(void)
{
    if(UART_GetITStatus(UART1, UART_IT_RXIEN) == SET)                           //check receive status
    {

        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        printBuf[rx_cnt % 100] = UART_ReceiveData(UART1);

        if( printBuf[rx_cnt % 100] == '\n')                                     //check '\n',and set send flag;
        {
            tx_flag = 1;
            pTx = printBuf;
        }
        rx_cnt++;
    }
    if(UART1->IER & UART_IT_TXIEN)                                              //judge send empty interrupt enable
    {
        if((UART_GetITStatus(UART1, UART_IT_TXIEN) == SET))
        {
            UartSendByte(*pTx++);
            UART_ClearITPendingBit(UART1, UART_IT_TXIEN);
            rx_cnt--;
            if(rx_cnt == 0)
            {
                UART_ITConfig( UART1,  UART_IT_TXIEN, DISABLE);
                tx_flag = 0;
            }
        }
    }
}

/*******************************************************************************
* @name   : NVIC_ConfigInit
* @brief  : UART NVIC Configure
* @param  : void
* @retval : void
*******************************************************************************/
void NVIC_ConfigInit(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;

    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_Init(& NVIC_InitStruct);

}

/*******************************************************************************
* @name   : RCC_ConfigInit()
* @brief  : RCC configure
* @param  : void
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
}

/*******************************************************************************
* @name   : Uart_ConfigInit()
* @brief  : UART1 initial
* @param  : void
* @retval : void
*******************************************************************************/
void Uart_ConfigInit(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStructure);

    UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
    UART_ITConfig( UART1,  UART_IT_RXIEN, ENABLE);

#if TEST_HALF_DATA != TEST_SELE_HALF_DATA_DISABLE
    SETnBIT(UART1->SCR, 12);
#endif

    UART_Cmd(UART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : UartSendByte()
* @brief  : UART1 send byte
* @param  : void
* @retval : void
*******************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData( UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

/*******************************************************************************
* @name   : UartSendGroup()
* @brief  : UART1 send byte
* @param  : u8* buf:buffer address
            u16 len:data length
* @retval : void
*******************************************************************************/
void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

/*******************************************************************************
* @name   : UartSendAscii()
* @brief  : UART1 send ASCII
* @param  : char *str
* @retval : void
*******************************************************************************/
void UartSendAscii(char *str)
{
    while(*str)
        UartSendByte(*str++);
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

