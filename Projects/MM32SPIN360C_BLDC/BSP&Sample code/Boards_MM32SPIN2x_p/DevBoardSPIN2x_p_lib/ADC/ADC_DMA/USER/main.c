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

//Reference voltage, the unit is: V
#define REFVOLATGE 3.3

//Used to store ADC conversion value
u16 ADCValue[20];
//Used to store the voltage value
float ADCVolatge;
//Used to store the value of the ADC second-order filter
u16 ADCFilterValue;
//DMA transfer completed flag
u8 ADCflag;
//Filter completed sign
u8 ADCFilterflag;

void GPIO_Configuration(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n);
void ADCInit(u8 ADC_Channel_x);
void DMAInit(void);
void ADCFilter(void);
void Get_ADCVolatge(void);

u32 adc_value = 0;

/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : None
* @retval : void
*******************************************************************************/
void InitSystick()
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

/*******************************************************************************
* @name   : SysTick_Handler
* @brief  : Systick interrupt
* @param  : None
* @retval : void
*******************************************************************************/
void SysTick_Handler()
{
    static u32 cnt;
    cnt++;
}

/*******************************************************************************
* @name   : main
* @brief  : ADC_DMA
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    DMAInit();
    ADCInit(ADC_Channel_2);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while(1)
    {
        //Determine whether to convert into the DMA interrupt 20 times
        if(ADCflag)
        {
            //Clear the conversion end flag
            ADCflag = 0;
            //20 times the data for second-order filtering
            ADCFilter();
            //Determine whether the second-order filter is over
            if(ADCFilterflag)
            {
                //Clear the filter end flag
                ADCFilterflag = 0;
                //Convert the filtered value to voltage
                Get_ADCVolatge();
                ADC_SoftwareStartConvCmd(ADC1, ENABLE);
            }
        }
    }
}
/*******************************************************************************
* @name   : GPIO_Configuration
* @brief  : Initialize GPIO
* @param  : GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n, GPIOMode_TypeDef GPIO_Mode (x can be A,B..D, n can be 0,1..15)
* @retval : void
*******************************************************************************/
void GPIO_Configuration(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_n;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : ADC1_SingleChannel
* @brief  : ADC initialization
* @param  : ADC_Channel_x (x can be 0,1..8)
* @retval : void
*******************************************************************************/
void ADCInit(u8 ADC_Channel_x)
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    GPIO_Configuration(GPIOA, GPIO_Pin_2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    //ADC prescale factor
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;
    //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;
    //AD data right-justified
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);

    //ADC_RegularChannelConfig(ADC, ADC_Channel_All_Disable, 0, ADC_SampleTime_13_5Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 0, ADC_SampleTime_239_5Cycles);

    if(ADC_Channel_x == ADC_Channel_14)
    {
        ADC_TempSensorCmd(ENABLE);
    }
    else if(ADC_Channel_x == ADC_Channel_15)
    {
        ADC_VrefintCmd(ENABLE);
    }

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
}

/*******************************************************************************
* @name   : DMAInit
* @brief  : Initialize DMA
* @param  : None
* @retval : void
*******************************************************************************/
void DMAInit(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    //DMA transfer peripheral address
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (ADC1->ADDATA);
    //DMA transfer memory address
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCValue;
    //DMA transfer direction from peripheral to memory
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //DMA cache size
    DMA_InitStructure.DMA_BufferSize = 20;
    //After receiving the data, the peripheral address is forbidden to move backward
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //After receiving the data, the memory address is shifted backward
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //Define the peripheral data width to 16 bits
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    //Define the memory data width to 16 bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //Cycle conversion mode
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //DMA priority is high
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    //DMA interrupt initialization
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/*******************************************************************************
* @name   : ADCFilter
* @brief  : Second order filtering
* @param  : None
* @retval : void
*******************************************************************************/
//ADC second-order filter, when the filter is completed erection filter end marker
void ADCFilter(void)
{
    static u16 cntFilter;
    static u32 lADCFilterValue = 0;
    for(cntFilter = 0; cntFilter < 20; cntFilter++)
    {
        lADCFilterValue += ADCValue[cntFilter];
    }
    cntFilter = 0;

    ADCFilterflag = 1;
    ADCFilterValue = (lADCFilterValue / 20 * 30 + ADCFilterValue * 70) / 100;
    lADCFilterValue = 0;
}

/*******************************************************************************
* @name   : Get_ADCVolatge
* @brief  : Convert the conversion data to voltage
* @param  : None
* @retval : void
*******************************************************************************/
void Get_ADCVolatge(void)
{
    ADCVolatge = ((float)ADCFilterValue / 4095) * REFVOLATGE;
}

/*******************************************************************************
* @name   : DMA1_Channel1_IRQHandler
* @brief  : When DMA send data, set ADCflag, PA8, clear the interrupt flag, stop the conversion
* @param  : None
* @retval : void
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    //Clear interrupt flag
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    ADCflag = 1;
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

