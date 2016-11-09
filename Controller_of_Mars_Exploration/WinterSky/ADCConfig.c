#include "ADCConfig.h"

#include "delay.h"

#include <stdio.h>

#include "WifiControl.h"


u16 AD_Value[10] = {0};

void ADC_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//����GPIOΪģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//ʹ�� ADC1 and GPIOC clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);              //72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	ADC_InitTypeDef ADC_InitStructure;
	
	//����ADC1, ����DMA, �������Լ�����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //ת����������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //ADC1�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 10;	   //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);		   //����ADC_InitStruct��ָ���Ĳ�������ʼ������ADC1�ļĴ���
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADCͨ��1,�������˳��ֵΪ1,����ʱ��Ϊ239.5����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 5, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 6, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 7, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 8, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 9, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 10, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);
	
	//Enable ADC1
	ADC_Cmd(ADC1, ENABLE);		  //ʹ��ADC1
	
	//Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);						//����ADC1��У׼�Ĵ���
	//Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));		//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�
	
	//Start ADC1 calibaration
	ADC_StartCalibration(ADC1);					//��ʼADC1��У׼״̬
	//Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));		//�ȴ�У׼���
	
	//Start ADC1 Software Conversion
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ADC1������ת����������
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	/* ADC1 DMA1 Channel Config */
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1); //��DMA ��ͨ��1 �Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA����ADC ����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA �ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = 10; //DMA ͨ����DMA ����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ���Ϊ16 λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ���Ϊ16 λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA ͨ�� x ӵ�и����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA ͨ��x û������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct ��ָ���Ĳ�����ʼ��DMA ��ͨ��
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}


u32 nTemp[10];

void ADC_Check(void) {
	for(int i = 0; i < 10; i++)
		nTemp[i] = 0;
	
	for(int i = 0; i < 256; i++) {
		for(int j = 0; j < 10; j++) {
			nTemp[j] += AD_Value[j];
		}
		delay_us(500);
	}
	
	char buf[70] = "value:";
	char* ptr = buf + 6;
	int value = 0;
	
	for(int i = 0; i < 10; i++) {
		value = nTemp[i]/256;
		
		for(int j = 1000; j > 0; j/=10) {
			*ptr++ = value/j + '0';
			value %= j;
		}
		*ptr++ = ',';
	}
	*ptr-- = 0;
	*ptr = '\n';
	printf("%s", buf);
}


void ADC_PrintValue(void) {
	char buf[70] = "value:";
	char* ptr = buf + 6;
	int value = 0;
	
	for(int i = 0; i < 10; i++) {
		value = AD_Value[i];
		
		for(int j = 1000; j > 0; j/=10) {
			*ptr++ = value/j + '0';
			value %= j;
		}
		*ptr++ = ',';
	}
	*ptr-- = 0;
	*ptr = '\n';
	printf("%s", buf);
}

