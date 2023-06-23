#include "systick.h"

extern void Systick_Callback(void);
extern u32 SystemCoreClock;

static __IO uint32_t s_wSysticks;

/**
* @brief  This function will be used to initialize SysTick.
* @param : None
* @retval : None
*/
void Systick_Init()
{
    /* SystemFrequency / 1000    1ms中断一次
    * SystemFrequency / 100000	 10us中断一次
    * SystemFrequency / 1000000 1us中断一次
    */
    SysTick_Config(SystemCoreClock / 1000);

    NVIC_SetPriority(SysTick_IRQn, 0);
}
void Inc_Systicks(void)
{
    s_wSysticks++;
}
uint32_t Get_Systicks(void)
{
    return s_wSysticks;
}
void Systick_Delay(__IO uint32_t Delay)
{
    uint32_t tickstart = 0;
    tickstart = Get_Systicks();
    while ((Get_Systicks() - tickstart) < Delay) {
    }
}
void Suspend_Systicks(void)
{
    CLEAR_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
}
void Resume_Systicks(void)
{
    SET_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
}

void SysTick_Handler(void)
{
    Inc_Systicks();
    Systick_Callback();
}
