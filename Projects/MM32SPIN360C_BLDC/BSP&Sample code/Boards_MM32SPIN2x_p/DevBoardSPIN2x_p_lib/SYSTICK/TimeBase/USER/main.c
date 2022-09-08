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
u16 fac_us;
u16 fac_ms;
void LED_Init(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);

/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : void
* @retval : void
*******************************************************************************/
void InitSystick()
{
    //Select external clock HCLK/8
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    fac_us = SystemCoreClock / 4000000;
    //Number of systick clocks required for each ms in the non-OS case
    fac_ms = (u16)fac_us * 1000;
}

/*******************************************************************************
* @name   : main
* @brief  : The LED flashing period is 2s
* @param  : void
* @retval : void
*******************************************************************************/
int main(void)
{
    InitSystick();
    LED_Init();

    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
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

/*******************************************************************************
* @name   : delay_us
* @brief  : delay us
* @param  : nus
* @retval : void
*******************************************************************************/
void delay_us(u32 nus)
{
    u32 temp;
    //Time loading
    SysTick->LOAD = nus * fac_us;
    //Clear counter
    SysTick->VAL = 0x00;
    //Countdown
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
        temp = SysTick->CTRL;
        //Waiting time arrives
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    //Close the counter
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    //Clear counter
    SysTick->VAL = 0X00;
}

/*******************************************************************************
* @name   : delay_ms
* @brief  : delay ms
* @param  : nms
* @retval : void
*******************************************************************************/
void delay_ms(u16 nms)
{
    u32 temp;
    //Time loading (SysTick->LOAD is 24bit)
    SysTick->LOAD = (u32)nms * fac_ms;
    //Clear counter
    SysTick->VAL = 0x00;
    //Countdown
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
    do
    {
        temp = SysTick->CTRL;
        //Waiting time arrives
    }
    while((temp & 0x01) && !(temp & (1 << 16)));
    //Close the counter
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    //Clear counter
    SysTick->VAL = 0X00;
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

