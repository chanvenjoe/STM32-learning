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
#include "stdio.h"

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?\
    (GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?\
    (GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?\
    (GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?\
    (GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))

////////////////////////////////////////////////////////////////////////////////

static __IO u32 TimingDelay;
void LED_Init(void);
void TimingDelay_Decrement(void);
void DelayMs(u16 nms);

/*******************************************************************************
* @name   : SysTick_Handler
* @brief  : SysTick Handler
* @param  : void
* @retval : void
*******************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();

}

/*******************************************************************************
* @name   : TimingDelay_Decrement
* @brief  : Timing Delay Decrement
* @param  : void
* @retval : void
*******************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
/*******************************************************************************
* @name   : DelayMs
* @brief  : delay ms
* @param  : nms
* @retval : void
*******************************************************************************/
void DelayMs(u16 nms)
{
    TimingDelay = nms;
    while(TimingDelay != 0);
}
/*******************************************************************************
* @name   : main
* @brief  : The LED flashing period is 2s
* @param  : void
* @retval : void
*******************************************************************************/
int main(void)
{
    if(SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }
    // Configure the SysTick handler priority
    NVIC_SetPriority(SysTick_IRQn, 0x0);
    LED_Init();
    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        DelayMs(1000);
    }
}

/*******************************************************************************
* @name   : LED_Init
* @brief  : Init LED
* @param  : void
* @retval : void
*******************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
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

