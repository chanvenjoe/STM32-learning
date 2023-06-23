////////////////////////////////////////////////////////////////////////////////
/// @file    uart.c
/// @author  AE TEAM
/// @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
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
#define _UART_C_

// Files includes
#include <stdio.h>
#include "uart.h"


////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_Exported_Functions
/// @{

extern u32 SystemCoreClock;

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE s32 __io_putchar(s32 ch)
#else
#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)

#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#else
void _sys_exit(s32 x)
{
    x = x;
}
//redefine fputc function
s32 fputc(s32 ch, FILE* f)
{
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#endif


void CONSOLE_Init(u32 baudrate)
{
    u32 tempBaud;

    RCC->APB2ENR |= RCC_APB2ENR_UART1;
    RCC->APB2RSTR |= RCC_APB2RSTR_UART1;
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_UART1);                                     //stop reset UART1

    tempBaud = (SystemCoreClock / baudrate) / 16;
    UART1->FRA = (SystemCoreClock / baudrate) % 16;                             //need UART1->GCR bit 0 is disanbe, then can set FRA register;
    UART1->BRR = tempBaud;
    UART1->CCR &= ~UART_CCR_CHAR;
    UART1->CCR |= UART_CCR_CHAR_8b;                                             //set UART1 as 8-N-1
    UART1->GCR = UART_GCR_RX | UART_GCR_TX | UART_GCR_UART;                     //enable receive and transmit , then enable UART1
    RCC->AHBENR |= RCC_AHBENR_GPIOA;
    //Set PA9 as AF1 UART1 TX PIN and PA10 as AF1 UART1 RX PIN
    GPIOA->AFRH &= ~((GPIO_AF_MODEMASK << GPIO_AFRH_AFR9_Pos) | \
                     (GPIO_AF_MODEMASK << GPIO_AFRH_AFR10_Pos)) ;

    GPIOA->AFRH |=  ((GPIO_AF_MODE1 << GPIO_AFRH_AFR9_Pos) | \
                     (GPIO_AF_MODE1 << GPIO_AFRH_AFR10_Pos));
    //Clear PA9,PA10 CNF MODE as defaut value
    GPIOA->CRH &= ~((GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_9_Pos) | \
                    (GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_10_Pos));

    //Set PA9 as AF push-pull output, Set PA10 as float input
    GPIOA->CRH |=  ((GPIO_CNF_MODE_AF_PP << GPIO_CRH_CNF_MODE_9_Pos) | \
                    (GPIO_CNF_MODE_FLOATING << GPIO_CRH_CNF_MODE_10_Pos));
}

/// @}


/// @}

/// @}

