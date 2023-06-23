#include "pwm.h"   

void Gate_Driver_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(BLDC_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GATE_DRIVER_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GATE_DRIVER_PORT, &GPIO_InitStructure);
    
    GATE_DRIVER_DISABLE();
}
void Bldc_Gpio_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //ʹ��BLDC��ӦPWM���IOʱ��
    RCC_AHBPeriphClockCmd(BLDC_GPIO_CLK, ENABLE);
    //����BLDC U�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_UH_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BLDC_UH_PORT, &GPIO_InitStructure);
    //����BLDC V�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_VH_PIN;
    GPIO_Init(BLDC_VH_PORT, &GPIO_InitStructure);
    //����BLDC W�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_WH_PIN;
    GPIO_Init(BLDC_WH_PORT, &GPIO_InitStructure);
    //����BLDC U�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_UL_PIN;
    GPIO_Init(BLDC_UL_PORT, &GPIO_InitStructure);
    //����BLDC V�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_VL_PIN;
    GPIO_Init(BLDC_VL_PORT, &GPIO_InitStructure);
    //����BLDC W�����Ŷ�ӦIOΪ�����������
    GPIO_InitStructure.GPIO_Pin = BLDC_WL_PIN;
    GPIO_Init(BLDC_WL_PORT, &GPIO_InitStructure);
    //����BLDCɲ�������ӦIOΪ�������빦��
    GPIO_InitStructure.GPIO_Pin = BLDC_BKP_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(BLDC_BKP_PORT, &GPIO_InitStructure);
    //����BLDC��ӦIO����ΪTIM��PWM����
    GPIO_PinAFConfig(BLDC_UH_PORT, BLDC_UH_PIN_SRC, BLDC_UH_PIN_AF);
    GPIO_PinAFConfig(BLDC_VH_PORT, BLDC_VH_PIN_SRC, BLDC_VH_PIN_AF);
    GPIO_PinAFConfig(BLDC_WH_PORT, BLDC_WH_PIN_SRC, BLDC_WH_PIN_AF);
    GPIO_PinAFConfig(BLDC_UL_PORT, BLDC_UL_PIN_SRC, BLDC_UL_PIN_AF);
    GPIO_PinAFConfig(BLDC_VL_PORT, BLDC_VL_PIN_SRC, BLDC_VL_PIN_AF);
    GPIO_PinAFConfig(BLDC_WL_PORT, BLDC_WL_PIN_SRC, BLDC_WL_PIN_AF);
    //����BLDCɲ�������ӦIO����ΪTIM_BKP����
    GPIO_PinAFConfig(BLDC_BKP_PORT, BLDC_BKP_PIN_SRC,BLDC_BKP_PIN_AF);
}

void Bldc_Pwm_Init(void)
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
    //����BLDC��ӦPWM��Ӧ���GPIO
    Bldc_Gpio_Config();
    //ʹ��BLDC_PWM_TIMʱ��
    BLDC_TIM_CLOCK_CMD(BLDC_TIM_CLK, ENABLE);
    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ Period
    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  Prescaler
    //����ʱ�ӷָ�:TDTS = TIM_CKD_DIV1
    //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period        = BLDC_TIM_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler     = BLDC_TIM_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = BLDC_TIM_REPETITIONCOUNTER;
    TIM_TimeBaseInit(BLDC_PWM_TIM, &TIM_TimeBaseStructure);

    //����ģʽ�¿���״̬ѡ��
    //����ģʽ�¿���״̬ѡ��
    //���������������:�����ر��ޱ���
    //DTG[7:0]��������������:(����ʱ��DT)
    TIM_BDTRInitStruct.TIM_OSSRState    = TIM_OSSRState_Enable;		
    TIM_BDTRInitStruct.TIM_OSSIState    = TIM_OSSIState_Enable;		
    TIM_BDTRInitStruct.TIM_LOCKLevel    = TIM_LOCKLevel_OFF;		
    TIM_BDTRInitStruct.TIM_DeadTime     = BLDC_TIM_DEADTIME;						
    //TDTS = 125nS(8MHz)
    //DTG[7: 5] = 0xx => DT = DTG[7: 0] * Tdtg, Tdtg = TDTS;
    //DTG[7: 5] = 10x => DT =(64+DTG[5: 0]) * Tdtg, Tdtg = 2 * TDTS;
    //DTG[7: 5] = 110 => DT =(32+DTG[4: 0]) * Tdtg, Tdtg = 8 * TDTS; 
    //DTG[7: 5] = 111=> DT =(32 + DTG[4: 0]) *  Tdtg, Tdtg = 16 * TDTS;

    //ɲ�����ã�ʹ��ɲ��
    //ɲ�����뼫��ѡ��:�͵�ƽ��Ч
    //�Զ����ʹ������:MOEֻ�������1
    TIM_BDTRInitStruct.TIM_Break            = TIM_Break_Disable;  			
    TIM_BDTRInitStruct.TIM_BreakPolarity    = TIM_BreakPolarity_Low;		
    TIM_BDTRInitStruct.TIM_AutomaticOutput  = TIM_AutomaticOutput_Enable;	
    TIM_BDTRConfig( BLDC_PWM_TIM, &TIM_BDTRInitStruct);
    //ģʽ���ã�PWMģʽ1
    //���״̬���ã�ʹ�����
    //����ͨ�����״̬���ã�ʹ�����
    //���ô�װ�벶��ȽϼĴ���������ֵ
    //����Ƚϼ��Ը�
    //N����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCMode 		    = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState    = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse           = 0;
    TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High;      
    TIM_OCInitStructure.TIM_OCNPolarity     = TIM_OCNPolarity_High; 
    TIM_OCInitStructure.TIM_OCIdleState     = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState    = TIM_OCNIdleState_Set;
    TIM_OC1Init(BLDC_PWM_TIM, &TIM_OCInitStructure);
    TIM_OC2Init(BLDC_PWM_TIM, &TIM_OCInitStructure);
    TIM_OC3Init(BLDC_PWM_TIM, &TIM_OCInitStructure);
    //ʹ��CH1��2��3Ԥװ��
    TIM_OC1PreloadConfig(BLDC_PWM_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(BLDC_PWM_TIM, TIM_OCPreload_Enable);  	
    TIM_OC3PreloadConfig(BLDC_PWM_TIM, TIM_OCPreload_Enable);
    //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(BLDC_PWM_TIM, ENABLE); 
    //ʹ��BLDC_PWM_TIM
    TIM_Cmd(BLDC_PWM_TIM, ENABLE);  			
    //�����ʹ�� ʹ��MOEλ
    TIM_CtrlPWMOutputs(BLDC_PWM_TIM, ENABLE);
    //�ر�CH1��2��3ͨ�����
    TIM_CCxCmd(BLDC_PWM_TIM,TIM_Channel_1,TIM_CCx_Disable);
    TIM_CCxNCmd(BLDC_PWM_TIM,TIM_Channel_1,TIM_CCxN_Disable);
    TIM_CCxCmd(BLDC_PWM_TIM,TIM_Channel_2,TIM_CCx_Disable);
    TIM_CCxNCmd(BLDC_PWM_TIM,TIM_Channel_2,TIM_CCxN_Disable);
    TIM_CCxCmd(BLDC_PWM_TIM,TIM_Channel_3,TIM_CCx_Disable);
    TIM_CCxNCmd(BLDC_PWM_TIM,TIM_Channel_3,TIM_CCxN_Disable);
}
