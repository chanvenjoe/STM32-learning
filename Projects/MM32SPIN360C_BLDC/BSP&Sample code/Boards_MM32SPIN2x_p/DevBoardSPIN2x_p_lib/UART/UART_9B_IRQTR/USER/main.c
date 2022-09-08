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

#define TX_IE_ENABLE 1
#if (TX_IE_ENABLE)
#define TX_CURR_ENABLE 0
#define RX_IE_ENABLE 0
#else

#define RX_IE_ENABLE 1

#define TX_CURR_ENABLE 1
#endif

#define TEST_SELE_B8EN_DISABLE          0
#define TEST_SELE_B8EN_ENABLE           1
#define TEST_SELE_B8TOG_DISABLE         0
#define TEST_SELE_B8TOG_ENABLE          1
#define TEST_SELT_B8POL_LOW             0
#define TEST_SELT_B8POL_HIGH            1
#define TEST_SELT_B8TXD_LOW             0
#define TEST_SELT_B8TXD_HIGH            1

#define TEST_SELE_B8EN                  TEST_SELE_B8EN_ENABLE
#define TEST_SELE_B8TOG                 TEST_SELE_B8TOG_DISABLE
#define TEST_SELT_B8POL                 TEST_SELT_B8POL_LOW
#define TEST_SELT_B8TXD                 TEST_SELT_B8TXD_LOW

char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);
void UartSendByte(u8 dat);

void RCC_ConfigInit(void);

void initGPIO_UART(void);
void uartConfig(unsigned int bound);
void NVIC_ConfigInit(void);

u16 rxdata;
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
            UART poll mode
            Send the received data
* @param  : void
* @retval : void
*******************************************************************************/
int main(void)
{
    InitSystick();
    RCC_ConfigInit();
    NVIC_ConfigInit();

    Uart_ConfigInit(9600);
    while(1)
    {

    }
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART1, &UART_InitStructure);

#if	(RX_IE_ENABLE)
    UART_ClearITPendingBit(  UART1, UART_IT_RXIEN);
    UART_ITConfig( UART1, UART_IT_RXIEN, ENABLE );
#endif
#if	(TX_IE_ENABLE)
    UART_ClearITPendingBit(  UART1, UART_IT_TXIEN);
    UART_ITConfig( UART1, UART_IT_TXIEN, ENABLE );
#endif

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#if TEST_SELE_B8EN !=  TEST_SELE_B8EN_DISABLE

#if TEST_SELT_B8TXD !=  TEST_SELT_B8TXD_LOW
    SETnBIT(UART1->CCR, 8);
#endif

#if TEST_SELT_B8POL !=  TEST_SELT_B8POL_LOW
    SETnBIT(UART1->CCR, 9);
#endif

#if TEST_SELE_B8TOG != TEST_SELE_B8TOG_DISABLE
    SETnBIT(UART1->CCR, 10);
#endif

    SETnBIT(UART1->CCR, 11);

#endif

    UART_Cmd(UART1, ENABLE);
}

/*******************************************************************************
* @name   : UART1_IRQHandler()
* @brief  : UART1 interrupt handler
* @param  : void
* @retval : void
*******************************************************************************/
int cnt = 0;
void UART1_IRQHandler(void)
{
#if	(TX_IE_ENABLE)
    if(UART_GetITStatus( UART1, UART_IT_TXIEN) )
    {
        UART_ClearITPendingBit(  UART1, UART_IT_TXIEN);
        UART1->TDR = cnt++;
    }
#endif

#if	(RX_IE_ENABLE)
    if(UART_GetITStatus( UART1, UART_IT_RXIEN) )
    {
        UART_ClearITPendingBit(  UART1, UART_IT_RXIEN);
#if (TX_CURR_ENABLE == 0)

        uartInfo.buf[ uartInfo.len ++ ] = UART_ReceiveData(UART1);
        uartInfo.interval = 0;
        uartInfo.dataFlag = 1;
#else
        UartSendByte(UART_ReceiveData(UART1));
#endif
    }
#endif
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
* @name   : UartSendByte()
* @brief  : UART1 send byte
* @param  : void
* @retval : void
*******************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData(UART1, dat);
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

