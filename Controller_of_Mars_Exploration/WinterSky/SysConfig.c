#include "SysConfig.h"


/*************************************************
����: void RCC_Configuration(void)
����: ��λ��ʱ�ӿ��� ����
����: ��
����: ��
**************************************************/
void RCC_Config(void) {
	ErrorStatus HSEStartUpStatus;                    //�����ⲿ���پ�������״̬ö�ٱ���
	RCC_DeInit();                                    //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
	RCC_HSEConfig(RCC_HSE_ON);                       //���ⲿ���پ���
	HSEStartUpStatus = RCC_WaitForHSEStartUp();      //�ȴ��ⲿ����ʱ��׼����
	
	if(HSEStartUpStatus == SUCCESS) {                 //�ⲿ����ʱ���Ѿ�׼���
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
		FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
      	
		RCC_HCLKConfig(RCC_SYSCLK_Div1);               //����AHB(HCLK)ʱ�ӵ���==SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);                //����APB2(PCLK2)��==AHBʱ��
		RCC_PCLK1Config(RCC_HCLK_Div2);                //����APB1(PCLK1)��==AHB1/2ʱ��
         
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //����PLLʱ�� == �ⲿ���پ���ʱ�� * 9 = 72MHz
		RCC_PLLCmd(ENABLE);                                   //ʹ��PLLʱ��
   
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);    //�ȴ�PLLʱ�Ӿ���
	  
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
		while(RCC_GetSYSCLKSource() != 0x08);                  //���PLLʱ���Ƿ���Ϊϵͳʱ��
	}
}

/*******************************************************************************
* Function Name   : NVIC_Configuration
* Description        : Configures NVIC and Vector Table base location.
* Input                    : None
* Output                 : None
* Return                 : None
*******************************************************************************/
void NVIC_Config(void) {
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
   /* Enable the USART1 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //ͨ������Ϊ����1�ж�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //�ж���Ӧ���ȼ�0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //���ж�
   NVIC_Init(&NVIC_InitStructure); 						   //��ʼ��
}
