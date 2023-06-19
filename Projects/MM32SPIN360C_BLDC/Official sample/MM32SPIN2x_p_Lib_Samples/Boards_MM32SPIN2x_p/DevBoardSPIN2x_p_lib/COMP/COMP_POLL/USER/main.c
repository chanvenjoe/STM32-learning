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
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);
void Comp_Config(uint32_t COMP_Selection_COMPx);
unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx);

void GPIO_ConfigInit(void);
void RCC_ConfigInit(void);

/*******************************************************************************
* @name   : main
* @brief  : COMP
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    Uart_ConfigInit(9600);
    RCC_ConfigInit();

    Comp_Config(COMP_Selection_COMP3);

    while(1)
    {

    }
}

/*******************************************************************************
* @name   : RCC_ConfigInit
* @brief  : RCC config
* @param  : None
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
* @name   : GPIO_ConfigInit
* @brief  : GPIO config
* @param  : None
* @retval : void
*******************************************************************************/
void GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
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
* @name   : Comp_Config
* @brief  : COMP config
* @param  : COMP_Selection_COMPx
* @retval : void
*******************************************************************************/
void Comp_Config(uint32_t COMP_Selection_COMPx)
{
    COMP_InitTypeDef COMP_InitStructure;
    GPIO_ConfigInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_CPT, ENABLE);

    COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_IO2;
    COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO2;
    COMP_InitStructure.COMP_Output = COMP_Output_None;
    COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
    COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
    COMP_InitStructure.COMP_Mode = COMP_Mode_MediumSpeed;

    COMP_Init(COMP_Selection_COMPx, &COMP_InitStructure);

    COMP_Cmd(COMP_Selection_COMPx, ENABLE);
}

/*******************************************************************************
* @name   : Comp_StatusCheck
* @brief  : Check COMP status
* @param  : COMP_Selection_COMPx
* @retval : void
*******************************************************************************/
unsigned char Comp_StatusCheck(uint32_t COMP_Selection_COMPx)
{
    unsigned char chCapStatus = 2;
    if(COMP_GetOutputLevel(COMP_Selection_COMPx) == 0)
    {
        chCapStatus = 0;
    }
    else
    {
        chCapStatus = 1;
    }
    return chCapStatus;
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

