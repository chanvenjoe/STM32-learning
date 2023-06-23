////////////////////////////////////////////////////////////////////////////////
/// @file     comp.c
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _COMP_C_

// Files includes
#include "comp.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup BKP_Exported_Constants
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  RCC configure
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void RCC_ConfigInit(void)
{
    RCC->AHBENR &= ~RCC_AHBENR_GPIOA;
    RCC->AHBENR |= RCC_AHBENR_GPIOA ;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  COMP configure
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void Comp_Config(void)
{
    RCC->APB2ENR &= ~(RCC_APB2ENR_COMP);  //COMP ENABLE
    RCC->APB2ENR |= RCC_APB2ENR_COMP;
    *(u32*)(COMP_BASE ) = COMP_CSR_INM_0 | COMP_CSR_INP_INP0 | COMP_CSR_OFLT_2 | COMP_CSR_MODE_MEDIUMRATE; //COMP config
    (*(u32*)(COMP_BASE  ) |=  COMP_CSR_EN);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  GPIO  configure initialization
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void COMPOut_GPIO_ConfigInit(void)
{
    RCC->APB2ENR &= ~(RCC_APB2Periph_SYSCFG);
    RCC->APB2ENR |= RCC_APB2Periph_SYSCFG;
    GPIOA->AFRL &= ~(GPIO_AFRL_AFR0);
    GPIOA->AFRL |= (GPIO_AF_MODE7 << GPIO_CRL_CNF_MODE_0_Pos);
    GPIOA->CRL &= ~(GPIO_CNF_MODE_MASK << GPIO_CRL_CNF_MODE_0_Pos); //IO status set
    GPIOA->CRL |= (GPIO_CNF_MODE_20MHZ_OUT_OD << GPIO_CRL_CNF_MODE_0_Pos); //IO status set
}
/// @}


/// @}

/// @}
