#include "SysConfig.h"


/*************************************************
函数: void RCC_Configuration(void)
功能: 复位和时钟控制 配置
参数: 无
返回: 无
**************************************************/
void RCC_Config(void) {
	ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
	RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
	RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
	HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
	
	if(HSEStartUpStatus == SUCCESS) {                 //外部高速时钟已经准别好
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
		FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
      	
		RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
		RCC_PCLK1Config(RCC_HCLK_Div2);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
		RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);    //等待PLL时钟就绪
	  
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
		while(RCC_GetSYSCLKSource() != 0x08);                  //检查PLL时钟是否作为系统时钟
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
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //通道设置为串口1中断
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
   NVIC_Init(&NVIC_InitStructure); 						   //初始化
}
