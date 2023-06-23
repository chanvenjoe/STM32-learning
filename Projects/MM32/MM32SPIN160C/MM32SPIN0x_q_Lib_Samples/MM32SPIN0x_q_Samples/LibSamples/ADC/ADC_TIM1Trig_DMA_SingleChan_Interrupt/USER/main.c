////////////////////////////////////////////////////////////////////////////////
/// @file    main.c
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
#define _MAIN_C_

// Files includes

#include "adc_tim1trig_dma_multichanit.h"
#include "led.h"
#include "main.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Example_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MAIN_Exported_Constants
/// @{

/// @}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This function is main entrance.
/// @param  None.
/// @retval  0.
////////////////////////////////////////////////////////////////////////////////
s32 main(void)
{
    u16 chan, averagenumber = 0;
    TIM_Trig_ADC_DMA_Init();
    while(1) {
        if(ADCflag == 1) {
            ADCflag = 0;
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            for(chan = 0; chan < ADCSCANNUM; chan++) {
                varADCavarage[averagenumber][chan] = ADCValue[chan];
            }
            averagenumber++;
            if(averagenumber >= AVERAGELEN) {
                averagenumber = 0;
                ADCTrigFilterflag = 1;
            }
        }
        //Determine whether the second-order filter is over
        if(ADCTrigFilterflag) {
            ADCFilter();
            //Clear the filter end flag
            ADCTrigFilterflag = 0;
            //Convert the filtered value to voltage
            Get_ADCVolatge();

        }

    }
}





/// @}

/// @}

/// @}
