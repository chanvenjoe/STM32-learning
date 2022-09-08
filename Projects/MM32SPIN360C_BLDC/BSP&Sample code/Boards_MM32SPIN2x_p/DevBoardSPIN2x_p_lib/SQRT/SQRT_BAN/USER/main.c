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
#include "stdio.h"
#include "HAL_device.h"
#include "HAL_conf.h"
#include "math.h"

char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);
void RCC_ConfigInit(void);
void GPIO_ConfigInit(void);


#define  SET_HWRoot(x)              HWROOT->SQR = x;
#define  GET_HWRoot                 HWROOT->ROOT;


u32 result[2];

/*******************************************************************************
* @name   : main
* @brief  : Square root
* @param  : None
* @retval : void
*******************************************************************************/
int main (void)
{
    int i;
    int errCnt = 0;
    RCC_ConfigInit();
    GPIO_ConfigInit();
    Uart_ConfigInit(9600);

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "sprintf ok\r\n"));

    for(i = 0; i < 0xFFFFFFFF; i ++)
    {


        SET_HWRoot(i);
        result[0] = GET_HWRoot;
        result[1] = (u32)(sqrt(i)) / 1;
        if(result[1] != result[0])
        {
            errCnt++;
            UartSendGroup((u8*)printBuf, sprintf( printBuf, "\r\n\r\nERROR\r\n\r\n" ));
            if(errCnt > 2)
            {
                while(1);
            }
        }
        if((i % 10000) == 0)
        {
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "i = %d\r\n", i));
        }

    }
    while(1);
}

/*******************************************************************************
* @name   : HWROOT_Calc
* @brief  : Calculate HWROOT
* @param  : DVD, DVS
* @retval : HWDIV->QUOT
*******************************************************************************/
u32 HWROOT_Calc(u32 DVD, u32 DVS)
{
    HWDIV->DVD = DVD;
    HWDIV->DVS = DVS;

    if(HWDIV->STATUS & 0x01)
    {
        return 0xffffffff;
    }
    return  HWDIV->QUOT;
}

/*******************************************************************************
* @name   : ROOT_Calc
* @brief  : Calculate ROOT
* @param  : DVD, DVS
* @retval : DVD/DVS
*******************************************************************************/
u32 ROOT_Calc(u32 DVD, u32 DVS)
{
    return  DVD / DVS;
}

/*******************************************************************************
* @name   : RCC_ConfigInit
* @brief  : RCC config
* @param  : None
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_HWROOT, ENABLE);
}

/*******************************************************************************
* @name   : GPIO_ConfigInit
* @brief  : GPIO config
* @param  : None
* @retval : void
*******************************************************************************/
void GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
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

