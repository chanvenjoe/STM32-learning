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

char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendByte(u8 dat);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);

void RCC_ConfigInit(void);
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
    InitSystick();
    RCC_ConfigInit();
    NVIC_ConfigInit();
    Uart_ConfigInit(2400);

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "sprintf ok\r\n"));
    UartSendAscii("UartSendAscii\r\n");
    UartSendAscii("Please input data ,end with '\\n' \r\n");

    while(1)
    {
        if(tx_flag != 0)
        {
            //clear send flag;
            UART_ITConfig(UART2,  UART_IT_TXIEN, ENABLE);
        }
    }
}
/*******************************************************************************
* @name   : UART2_IRQHandler
* @brief  : UART2 interrupt handler
* @param  : void
* @retval : void
*******************************************************************************/
void UART2_IRQHandler(void)
{
    //check receive status
    if(UART_GetITStatus(UART2, UART_IT_RXIEN) == SET)
    {
        UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
        printBuf[rx_cnt % 100] = UART_ReceiveData(UART2);
        //check '\n',and set send flag;
        if( printBuf[rx_cnt % 100] == '\n')
        {
            tx_flag = 1;
            pTx = printBuf;
        }
        rx_cnt++;
    }
    //judge send empty interrupt enable
    if(UART2->IER & UART_IT_TXIEN)
    {
        if((UART_GetITStatus(UART2, UART_IT_TXIEN) == SET))
        {
            UartSendByte(*pTx++);
            UART_ClearITPendingBit(UART2, UART_IT_TXIEN);
            rx_cnt--;
            if(rx_cnt == 0)
            {
                UART_ITConfig(UART2, UART_IT_TXIEN, DISABLE);
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

    NVIC_InitStruct.NVIC_IRQChannel = UART2_IRQn;
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
* @brief  : UART2 initial
* @param  : void
* @retval : void
*******************************************************************************/
void Uart_ConfigInit(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART2, &UART_InitStructure);

    UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);

    UART_Cmd(UART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : UartSendByte()
* @brief  : UART2 send byte
* @param  : void
* @retval : void
*******************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData(UART2, dat);
    while(!UART_GetFlagStatus(UART2, UART_FLAG_TXEPT));
}

/*******************************************************************************
* @name   : UartSendGroup()
* @brief  : UART2 send byte
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
* @brief  : UART2 send ASCII
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

