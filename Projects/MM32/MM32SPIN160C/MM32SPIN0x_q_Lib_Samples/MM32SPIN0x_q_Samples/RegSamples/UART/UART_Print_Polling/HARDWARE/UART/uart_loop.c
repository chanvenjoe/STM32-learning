////////////////////////////////////////////////////////////////////////////////
/// @file     uart_loop.c
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
#define _UART_LOOP_C_

// Files includes
#include "sys.h"
#include "uart_loop.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_LOOP
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_LOOP_Exported_Functions
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @brief  uart initialization
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void UartInit_Loop(void)
{
    //GPIO port set
    RCC->AHBENR |= RCC_AHBENR_GPIOA; //enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_UART1; //enable uart clock
    GPIOA->AFRH &= (~(GPIO_AFRH_AFR9 | GPIO_AFRH_AFR10));
    GPIOA->AFRH |= (GPIO_AF_MODE1 << GPIO_AFRH_AFR9_Pos) | (GPIO_AF_MODE1 << GPIO_AFRH_AFR10_Pos);
    //UART initialset
    UART1->BRR = (SystemCoreClock / 115200) / 16;
    UART1->FRA = (SystemCoreClock / 115200) % 16;
    UART1->CCR |= UART_CCR_CHAR_8b; //110000
    UART1->GCR |= UART_GCR_TX | UART_GCR_RX | UART_GCR_UART; //11001;
    GPIOA->CRH &= ~((GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_10_Pos) | (GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_9_Pos)); //IO status set
    GPIOA->CRH |= (GPIO_CNF_MODE_FLOATING << GPIO_CRH_CNF_MODE_10_Pos) | (GPIO_CNF_MODE_AF_PP << GPIO_CRH_CNF_MODE_9_Pos);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  uart sent byte
/// @param  UARTx : x can be 1 or 2 or 3
/// @param  c :uart sent byte
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void uart_send(s8 c)
{
    UART1->TDR = c;
    while(1) {
        if(UART1->ISR & UART_ISR_TX) {
            UART1->ICR = UART_ICR_TX;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  uart receive function
/// @param  UARTx:x can be 1 or 2 or 3
/// @retval None
////////////////////////////////////////////////////////////////////////////////
u8 inbyte(UART_TypeDef* UARTx)
{
    u8 temp;
    while(1) {
        if(UART1->ISR & UART_ISR_RX) {
            UART1->ICR = UART_ICR_RX;
            break;
        }
    }
    temp = (u8)UART1->RDR;
    if(temp == 0xd) { //clear  error receive  data
        return 0;
    }
    return temp;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  uart receive function  test
/// @param  UARTx , x can be 1 or 2 or 3
/// @retval None
////////////////////////////////////////////////////////////////////////////////

void Uart1RxTest(UART_TypeDef* UARTx)
{
    u8 temp;
    temp = inbyte(UARTx);
    if(temp != 0) {

    }
}
/// @}


/// @}

/// @}





