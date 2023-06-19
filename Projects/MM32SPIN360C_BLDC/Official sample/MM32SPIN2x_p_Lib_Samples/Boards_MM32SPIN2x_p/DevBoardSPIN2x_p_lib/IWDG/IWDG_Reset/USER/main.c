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


#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)    // PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15) // PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3) // PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)  // PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))   // PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4) // PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)  // PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))   // PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5) // PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)  // PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))   // PB5


void LED_Init(void);
char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendByte(u8 dat);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);

void PVU_CheckStatus(void);
void RVU_CheckStatus(void);
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload);
void Write_Iwdg_RL(void);
static void DelayLoop(u32 uldelay);

/*******************************************************************************
* @name   : main
* @brief  : IWDG
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    u32 ulA = 0;
    u32 ulDelay = 0;
    LED_Init();
    Uart_ConfigInit(9600);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "IWDG RESET\r\n"));
    ulDelay = 100;
    while (1)
    {
        ulA++;
        DelayLoop(ulDelay);
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        if (ulA == 50)
        {
            ulA = 0;
            break;
        }
    }

    //Configured for LSI32 frequency division, the initial value of the counter is 0xf.
    Write_Iwdg_ON(IWDG_Prescaler_32, 0xf);
    ulDelay = 250;
    ulA = 0;
    while (1)
    {
        ulA++;
        DelayLoop(4);
        if(ulDelay < ulA)
        {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            ulA = 0;
        }
        //No reset program enters an infinite loop. The system always prints serial data anyway.
//        Write_Iwdg_RL(); //if comment this Line, MCU always reset;
    }
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
* @name   : PVU_CheckStatus
* @brief  : Check IWDG prescaler status
* @param  : None
* @retval : void
*******************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        //Check the prescaler bit status. Only RESET can change the prescaler value.
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET)
        {
            break;
        }
    }
}

/*******************************************************************************
* @name   : RVU_CheckStatus
* @brief  : Check IWDG reload status
* @param  : None
* @retval : void
*******************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        //Check overload flag status
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET)
        {
            break;
        }
    }
}

/*******************************************************************************
* @name                 : Write_Iwdg_PR
* @brief                : Start IWDG
* @param IWDG_Prescaler : IWDG_Prescaler_X, X is 4,8,16,32,64,128,256, corresponding to the same value as the X value
* @param Reload         : Counter reload value
* @retval               : void
*******************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload)
{
    /*Start the internal low-speed clock and wait for the clock to be ready*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /*Set clock prescaler*/
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    PVU_CheckStatus();
    /*Set the overload register value*/
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);
    RVU_CheckStatus();
    /*Load and enable counters*/
    IWDG_ReloadCounter();
    IWDG_Enable();
    DelayLoop(20);
}

/*******************************************************************************
* @name   : Write_Iwdg_RL
* @brief  : Reload function
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}


static void DelayLoop(u32 uldelay)
{
    u32 i, j;
    for (i = 0; i < uldelay; i++)
    {
        for (j = 0; j < 3000; j++)
        {

        }
    }
}
/********************************************************************************************************
**函数信息 ：LED_Init(void)
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);   //开启GPIOA,GPIOB时钟

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
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

