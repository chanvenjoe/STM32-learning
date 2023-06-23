////////////////////////////////////////////////////////////////////////////////
/// @file     sys.c
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
#define _SYS_C_

// Files includes
#include <string.h>
#include "mm32_device.h"


#include "sys.h"




////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SYS
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SYS_Exported_Constants
/// @{

/// @}




#define CUSTOM_HSEFREQ                  8000000UL

#define CUSTOM_LATENCY                  0
#define CUSTOM_PLLMUL                   1
#define CUSTOM_PLLDIV                   2
#define CUSTOM_SYSCLKSRC                2
#define CUSTOM_PLLSRC                   0
#define CUSTOM_SYSTICK_FREQ             1000

#define HSI_Checking_Counter            2
#define HSI_TIMEROUT_Counter            1000

#define HSE_Checking_Counter            10
#define HSE_TIMEROUT_Counter            5000

#define LSI_Checking_Counter            2
#define LSI_TIMEROUT_Counter            1000

#define PLL_Checking_Counter            2
#define PLL_TIMEROUT_Counter            1000

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Exported_Functions
/// @{


u32 RCC_ClkPrescaler_Table[] = {
    // HAPB2                      LAPB1                         AHB/1                       AHB2                  AHB3
    RCC_CFGR_PPRE2_DIV1,       RCC_CFGR_PPRE1_DIV2,          RCC_CFGR_HPRE_DIV1          //  0,                   0,  // HSI_6d_1x,                 //  SYSTEMCLK_HSI_8MHz,
};
ErrorStatus exRCC_ClkPrescaler_Init(u8  SystemClkSel)
{
    // AHB, APB1, APB2
    RCC->CFGR |= (RCC_ClkPrescaler_Table[2]) & RCC_CFGR_HPRE;
    RCC->CFGR |= (RCC_ClkPrescaler_Table[1]) & RCC_CFGR_PPRE1;
    RCC->CFGR |= (RCC_ClkPrescaler_Table[0]) & RCC_CFGR_PPRE2;
    return SUCCESS;
}
static void udelay(void)
{
    __IO u32 i;
    i = 256;
    while (i--);
}


ErrorStatus Sysclk_Init(SYSTEMCLK_TypeDef  SystemClkSel)
{

    u32 CheckingCounter = 0;
    u32 TimerOutCounter = 0;
//------------------------------------------------------------------------------
    u8 Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[] = {
        //| latency      | PLLMUL     |    PLLDIV     | SYSCLK          |   PLLSRC
        0,              0,              0,          0,              0,      // HSI_6d_1x,       //  SYSTEMCLK_HSI_8MHz,
        0,              0,              0,          0,              3,      // HSI_6d_1xh,      //  SYSTEMCLK_HSI_12MHz,
        1,              0,              0,          2,              0,      // HSI_1d_1x,       //  SYSTEMCLK_HSI_48MHz,
        2,              0,              0,          2,              3,      // HSI_1d_1xh,      //  SYSTEMCLK_HSI_72MHz,
        0,              0,              0,          1,              0,      // HSE_1x,          //  SYSTEMCLK_HSE_8MHz,         Based HSE = 8MHz
        0,              0,              0,          3,              0,      // LSI_1x,          //  SYSTEMCLK_LSI_40KHz,
        //| latency      | PLLMUL     |    PLLDIV     | SYSCLK          |   PLLSRC
        CUSTOM_LATENCY, CUSTOM_PLLMUL, CUSTOM_PLLDIV, CUSTOM_SYSCLKSRC, CUSTOM_PLLSRC  // Custom_Freq,  //  SYSTEMCLK_CUSTOM_Freq,
    };
//SYSCLKSRC   0 -- HSI/6  ,  1 HSE   2 HSI/1   3 LSI
//PLLSRC      0 -- HSI_48 ,  1 HSI72

    u8 Latency, PllMul, PllDiv, SysclkSrc, PllSrc;
    if(SystemClkSel > (sizeof(Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table) / 5)) {
        SystemClkSel = SYSTEMCLK_HSI_8MHz;
    }
    //pArray = &Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB];
    Latency   = Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB];
    PllMul    = Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB + 1];
    PllDiv    = Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB + 2];
    SysclkSrc = Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB + 3];
    PllSrc    = Latency_PllMul_PllDiv_SysclkSrc_PllSrc_Table[SystemClkSel * RCC_ROW_TB + 4];
    //------------------------------------------------------------------------------

    SystemClockValue = 8000000UL;                                  // default clock
    RCC->CR |= RCC_CR_HSION;

    CheckingCounter = HSI_Checking_Counter;
    TimerOutCounter = HSI_TIMEROUT_Counter;
    while (1) {
        udelay();
        if (TimerOutCounter == 0) {
            return ERROR;
        }
        TimerOutCounter--;
        if (RCC->CR & RCC_CR_HSIRDY) {
            if (CheckingCounter == 0 ) {
                break;
            }
            CheckingCounter--;
        }
    }
    // Clock Switch to
    RCC->CFGR |= (0 << RCC_CFGR_SW_Pos) & RCC_CFGR_SW;

    while (((RCC->CFGR & RCC_CFGR_SWS) >> 2) != 0);

    // Flash
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= Latency;
    exRCC_ClkPrescaler_Init(0);


    if  ( (SysclkSrc == 1) || (PllSrc == 1) || (PllSrc == 2) ) {
        //use HSE
        RCC->CR |= RCC_CR_HSEON;
        CheckingCounter = HSE_Checking_Counter;
        TimerOutCounter = HSE_TIMEROUT_Counter;
        while (1) {
            udelay();
            if (TimerOutCounter == 0) {
                return ERROR;
            }
            TimerOutCounter--;
            if (RCC->CR & RCC_CR_HSERDY) {
                if (CheckingCounter == 0 ) {
                    break;
                }
                CheckingCounter--;
            }
        }
        SystemClockValue = CUSTOM_HSEFREQ;
    }

    if   (SysclkSrc == 3)  {
        RCC->CSR |= RCC_CSR_LSION;

        CheckingCounter = LSI_Checking_Counter;
        TimerOutCounter = LSI_TIMEROUT_Counter;
        while (1) {
            udelay();
            if (TimerOutCounter == 0) {
                return ERROR;
            }
            TimerOutCounter--;
            if (RCC->CSR & RCC_CSR_LSIRDY) {
                if (CheckingCounter == 0 ) {
                    break;
                }
                CheckingCounter--;
            }
        }
        SystemClockValue = 40000;
    }

    if(SysclkSrc == 2) {

        if (((SCB->CPUID & COREID_MASK) == 0) && ((u32) * ((u32*)(0x40013400))  == (0xCC4460B1U)) ) {

            if(PllSrc == 3) {
                RCC->CR |=  (0x01U << 20);   //set   RCC_CR_HSI_72M bit
                SystemClockValue = 72000000UL;
            }
            else {
                RCC->CR &= ~(0x01U << 20);   //reset RCC_CR_HSI_72M bit
                SystemClockValue = 48000000UL;
            }
            PllMul = PllMul;
            PllDiv = PllDiv;
        }
        else {
            while(1);//MCU is not M0 q version, please check Core #define and Target MCU
        }
//------------------------------------------------------------------------------

    }
    // Clock Switch to
    RCC->CFGR |= (((u32)SysclkSrc) << RCC_CFGR_SW_Pos) & RCC_CFGR_SW;
    while (((RCC->CFGR & RCC_CFGR_SWS) >> 2) != ((u32)SysclkSrc & RCC_CFGR_SW));

    return SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Resets the RCC clock configuration to default state.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RCC_SetDefault(void)
{
    SET_BIT(RCC->CR, RCC_CR_HSION);
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);


    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON );
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    CLEAR_REG(RCC->CFGR);
    CLEAR_REG(RCC->CIR);
}
////////////////////////////////////////////////////////////////////////////////
///  @brief  Setup the microcontroller system
///          Initialize the Embedded Flash Interface, the PLL and update the
///  @param  None.
///  @retval None.
////////////////////////////////////////////////////////////////////////////////

void System_Clock_Init(SYSTEMCLK_TypeDef  System_Clock_Enum)
{
    RCC_SetDefault();
    Sysclk_Init(System_Clock_Enum);
}


/// @}


/// @}

/// @}
