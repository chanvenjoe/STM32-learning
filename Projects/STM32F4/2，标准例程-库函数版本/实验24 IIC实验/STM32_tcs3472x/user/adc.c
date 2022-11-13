

/******************************************************************************/
void ADC1_2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADCʱ�ӣ�ΪPCLK2��6��Ƶ����12MHz

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;         //PA7��AD_V
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    //PB0��AD_U��PB1��ص�ѹ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   // ��������ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);            // ��ʼ��
 
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ad_value; //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����(�����赽�ڴ�)
	DMA_InitStructure.DMA_BufferSize = 16; //�������ݵĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Զ�����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;    //�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1��1ͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);
 
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1ͬ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;                                       //ת��ͨ����4
	ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1
	ADC_TempSensorVrefintCmd(ENABLE);                                           //�����ڲ��¶ȴ�����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_7Cycles5);    //AD_V
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16,2, ADC_SampleTime_7Cycles5);  //�¶�  ADCx_IN16��ADCx_IN17ֻ�ܳ�������ADC1��
	ADC_DMACmd(ADC1, ENABLE); 

	ADC_DeInit(ADC2);
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;                                       //ת��ͨ����
	ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_7Cycles5);    //AD_U
	ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 2, ADC_SampleTime_7Cycles5);    //AD_Bat
	ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //ʹ��ADC2���ⲿ����ģʽ 

	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);//��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�У׼�Ĵ�����λ���
	ADC_StartCalibration(ADC1);//ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼���

	ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2
	ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //�������ADC1����ʼת��
}
/******************************************************************************/



