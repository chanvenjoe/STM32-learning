////////////////////////////////////////////////////////////////////////////////
/// @file     UART_NVIC.C
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
#define _UART_NVIC_C_

// Files includes
#include "uart_nvic.h"
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_NVIC
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup UART_NVIC_Exported_Functions
/// @{


#if EN_UART1_RX   //if enable receive 

u8 UART_RX_BUF[UART_REC_LEN];     // receive  buffer ,maximum UART_REC_LEN byte.

u16 UART_RX_STA = 0;     // receive  status  flag


/////////////////////////////////////////////////////////////////////////////////
/// @brief  setup NVIC Initialize
/// @param  NVIC_IRQChannelPriority,
/// @param  NVIC_IRQChannel,
/// @param  NVIC_IRQChannelCmd
/// @retval None
/////////////////////////////////////////////////////////////////////////////////
void M0_NVIC_Init(u32 NVIC_IRQChannelPriority, IRQn_Type NVIC_IRQChannel, FunctionalState NVIC_IRQChannelCmd)
{
    if (NVIC_IRQChannelCmd != DISABLE) {
        NVIC->IP[NVIC_IRQChannel >> 0x02] =
            (NVIC->IP[NVIC_IRQChannel >> 0x02] &
             (~(((u32)0xFF) << ((NVIC_IRQChannel & 0x03) * 8)))) |
            ((((u32)NVIC_IRQChannelPriority << 6) & 0xFF) << ((NVIC_IRQChannel & 0x03) * 8));

        NVIC->ISER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
    else {
        NVIC->ICER[0] = 0x01 << (NVIC_IRQChannel & 0x1F);
    }
}
void uart_nvic_init(u32 baudrate)
{
    u32 tempBaud;
    //-------------------------------------------------
    RCC->AHBENR |= RCC_AHBENR_GPIOA; //enablePORTA clock
    //enable GPIOA clock(RCC_AHBENR_GPIOA = 1 << 17)
    RCC->APB2ENR |= RCC_APB2ENR_UART1;
    //enable uart clock(RCC_APB2ENR_UART1= 1 << 14)
    RCC->APB2RSTR |= RCC_APB2RSTR_UART1;
    //reset   uart 1(RCC_APB2RSTR_UART1 =1 << 14)
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_UART1);
    GPIOA->AFRH &= (~(GPIO_AFRH_AFR9 | GPIO_AFRH_AFR10));
    GPIOA->AFRH |= (GPIO_AF_MODE1 << GPIO_AFRH_AFR9_Pos) | (GPIO_AF_MODE1 << GPIO_AFRH_AFR10_Pos);
    //-------------------------------------------------
    // Determine the uart_baud
    tempBaud = (SystemCoreClock / baudrate) / 16;
    UART1->FRA = (SystemCoreClock / baudrate) % 16;
    // Write to UART BRR
    UART1->BRR = tempBaud;
    UART1->CCR |= UART_CCR_CHAR_8b; //1bit stop,none verify bit .
    //-------------------------------------------------
#if EN_UART1_RX       //if enable receive 
    //enable receive interrupt
    UART1->GCR = UART_GCR_TX | UART_GCR_RX | UART_GCR_UART;      // receive and sent enable  UART1->CCR|=1<<5;    // receive  buffer area register  null interrupt enable
    UART1->IER = UART_IER_RX;           // receive interrupt enable
    //
    //23.5.5 UART interrupt enable device(UART_IER)
    //offset address :0x10
    //reset  value :0x0000
    //bit 1
    //RXIEN: receive  buffer interrupt enablebit
    //1=interrupt enable
    //0=interrupt forbid
    //
    UART1->ICR = UART_ICR_RX;           //clear  receive interrupt
    //
    //23.5.6 UART interrupt clear  device(UART_ICR)
    //offset address :0x14
    //reset  value :0x0000
    //bit 1
    //RXICLR:  receive interrupt clear bit
    //1=interrupt clear
    //0=interrupt not clear
    //

    M0_NVIC_Init(2, UART1_IRQn, ENABLE);
#endif
    //
    //bit 14
    //UART1EN:UART1clock enable
    //Soft set '1' or clear '0'
    //0:UART1 clock  close ;
    //1:UART1 clock start .
    //
    GPIOA->CRH &= ~((GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_10_Pos) | (GPIO_CNF_MODE_MASK << GPIO_CRH_CNF_MODE_9_Pos)); //IO status set
    GPIOA->CRH |= (GPIO_CNF_MODE_FLOATING << GPIO_CRH_CNF_MODE_10_Pos) | (GPIO_CNF_MODE_AF_PP << GPIO_CRH_CNF_MODE_9_Pos);

}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief  uart1 interrupt handler
/// @param  None
/// @retval None
////////////////////////////////////////////////////////////////////////////////
void UART1_IRQHandler(void)                 // uart 1interrupt procedure
{
    u8 Res;
    if((UART1->ISR & UART_ISR_RX) != (u16)RESET) { // receive  to  data
        Res = (u16)(UART1->RDR & (u16)UART_RDR_DATA);
        UART1->ICR |= 2;//clear  receive interrupt  //      UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        if((Res == 0x0d) && ((UART_RX_STA & 0X3FFF) > 0)) {
            UART_RX_STA |= 0x4000;
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        }
        else if((UART_RX_STA & 0x4000) && ((UART_RX_STA & 0X3FFF) > 0)) { // received0x0d
            if(Res == 0x0a) {
                UART_RX_STA |= 0x8000;
            }
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        }
        else {
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
            UART_RX_STA = UART_RX_STA & 0X3FFF;
            if((UART_RX_STA) > (UART_REC_LEN - 1))
                UART_RX_STA = 0; // receive  data  error, restart receiving
        }
    }
}
/// @}


/// @}

/// @}

