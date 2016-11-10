#include "ADCConfig.h"

#include "delay.h"

#include <stdio.h>

#include "WifiControl.h"


u16 AD_Value[10] = {0};

void ADC_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置GPIO为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//使能 ADC1 and GPIOC clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);              //72M/6=12,ADC最大时间不能超过14M
	
	ADC_InitTypeDef ADC_InitStructure;
	
	//配置ADC1, 不用DMA, 用软件自己触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  //转换由软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	 //ADC1数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 10;	   //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);		   //根据ADC_InitStruct中指定的参数，初始化外设ADC1的寄存器
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道1,规则采样顺序值为1,采样时间为239.5周期
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
	ADC_Cmd(ADC1, ENABLE);		  //使能ADC1
	
	//Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);						//重置ADC1的校准寄存器
	//Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));		//获取ADC1重置校准寄存器的状态,设置状态则等待
	
	//Start ADC1 calibaration
	ADC_StartCalibration(ADC1);					//开始ADC1的校准状态
	//Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));		//等待校准完成
	
	//Start ADC1 Software Conversion
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能ADC1的软件转换启动功能
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	/* ADC1 DMA1 Channel Config */
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1); //将DMA 的通道1 寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC 基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA 内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = 10; //DMA 通道的DMA 缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16 位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA 通道 x 拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA 通道x 没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct 中指定的参数初始化DMA 的通道
	
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


