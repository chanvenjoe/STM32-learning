#include "hall.h"

void Hall_Init(void)
{
    NVIC_InitTypeDef        NVIC_InitStructure;
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef       TIM_ICInitStructure;
    
    RCC_AHBPeriphClockCmd(HALL_GPIO_CLK,ENABLE);
    HALL_TIM_CLOCK_CMD(HALL_TIM_CLK,ENABLE);
    
    GPIO_PinAFConfig(HALL_PORT_U, HALL_PINSOURCE_U, HALL_U_PIN_AF);
    GPIO_PinAFConfig(HALL_PORT_V, HALL_PINSOURCE_V, HALL_V_PIN_AF);
    GPIO_PinAFConfig(HALL_PORT_W, HALL_PINSOURCE_W, HALL_W_PIN_AF);
    
    GPIO_InitStructure.GPIO_Pin = HALL_PIN_U;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
    GPIO_Init(HALL_PORT_U, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HALL_PIN_V; 
    GPIO_Init(HALL_PORT_V, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HALL_PIN_W; 
    GPIO_Init(HALL_PORT_W, &GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = HALL_TIM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
    NVIC_Init(&NVIC_InitStructure);

    /* ��ʱ����������ʼ�� */		 
    /* ��ʱ����: HALL_TIM_Period+1 */
    TIM_TimeBaseStructure.TIM_Period = HALL_TIM_PERIOD; 
    /* ����Ԥ��Ƶ��HALL_TIM_Prescaler,�������Ƶ�ʣ�72MHz/(HALL_TIM_Prescaler+1)/(HALL_TIM_Period+1) */
    TIM_TimeBaseStructure.TIM_Prescaler = HALL_TIM_PRESCALER;
    /* ����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���) */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
    /* ���ϼ���ģʽ */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseInit(HALL_TIM, &TIM_TimeBaseStructure);

    /* ��ʼ��TIM5���벶����� */
    /* CC1S=01 	ѡ������� IC1ӳ�䵽TI1�� */
    TIM_ICInitStructure.TIM_Channel     = HALL_TIM_Channel_x;
    /* �����ز��� */
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_BothEdge;	
    /* ӳ�䵽TI1�� */
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC; 
    /* ���������Ƶ,����Ƶ  */
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	           
    /* IC1F=0000 ���������˲��� ���˲� */
    TIM_ICInitStructure.TIM_ICFilter    = 0x08;                         
    TIM_ICInit(HALL_TIM, &TIM_ICInitStructure);  

    TIM_SelectHallSensor(HALL_TIM,ENABLE);                  //ʹ��TIMx�Ļ����������ӿ�
    TIM_SelectInputTrigger(HALL_TIM, TIM_TS_TI1F_ED);       //���봥��Դѡ��     

    TIM_SelectSlaveMode(HALL_TIM, TIM_SlaveMode_Reset);     //��ģʽѡ��
    TIM_SelectMasterSlaveMode(HALL_TIM, TIM_MasterSlaveMode_Enable);    //����ģʽѡ��    
    /* ��������ж� ,����CC1IE�����ж�	 */
    TIM_ITConfig(HALL_TIM, TIM_IT_Trigger, ENABLE);
    /* ʹ�ܶ�ʱ�� */
    TIM_Cmd(HALL_TIM, ENABLE);
    TIM_ClearITPendingBit(HALL_TIM,TIM_IT_Trigger);
    
}

