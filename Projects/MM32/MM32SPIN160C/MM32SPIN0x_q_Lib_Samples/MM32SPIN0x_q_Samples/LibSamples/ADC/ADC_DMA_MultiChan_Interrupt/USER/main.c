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


#include "main.h"
#include "adc_dma_multichanit.h"
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
    DMAInit();
    ADCConfig(ADCch0);
    ADCConfig(ADCch4);
    ADCConfig(ADCch5);
    //Start Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(1) {
        if(ADCflag == 1) {
            ADCflag = 0;
            for(chan = 0; chan < ADCSCANNUM; chan++) {
                varADCavarage[averagenumber][chan] = ADCValue[chan];
            }
            ADC_SoftwareStartConvCmd(ADC1, ENABLE);
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
