 /******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
**********************************************************************************/
#include "UsartConf.h"
#include "LedConf.h"
#include <stdarg.h>

u8 RcvBuf[256] = {'0'};
u8 RcvStart = 0, RcvEnd = 0, RcvNext = 1;


/*******************************************************************************
	函数名：USART_Configuration
	输  入:
	输  出:
	功能说明：
	初始化串口硬件设备，启用中断
	配置步骤：
	(1)打开GPIO和USART的时钟
	(2)设置USART两个管脚GPIO模式
	(3)配置USART数据格式、波特率等参数
	(4)使能USART接收中断功能
	(5)最后使能USART功能
*/
void USART1_Config(void) {
	LED_Config();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* 第4步：配置USART参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* 若接收数据寄存器满，则产生中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART1, ENABLE);

	/* 如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     // 清标志
	
	//缓冲初始化
	for(u8 i = 0; i != 0; i++) RcvBuf[i] = i;
}

/* NVIC全局只配置一次
void NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
  
	Set the Vector Table base location at 0x08000000
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
	Configure the NVIC Preemption Priority Bits
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
	Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
	NVIC_Init(&NVIC_InitStructure); 						   //初始化
}
*/

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f) {
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix) {
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10) {
	    *ptr = 0;
	    return string;
	}
	
	if (!value) {
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0) {
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10) {
	    d = value / i;
	
	    if (d || flag) {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */


/*
 * 函数名：USART_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART_printf( USART1, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...) {
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0) {     // 判断是否到达字符串结束符
		if ( *Data == 0x5c ) {  //'\'
			switch ( *++Data ) {
			case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;
				
			case 'n':							          //换行符
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;
				
			default:
				Data ++;
				break;
			}
		} else if ( *Data == '%') {
			switch ( *++Data ) {
			case 's':										  //字符串
				s = va_arg(ap, const char *);
				for ( ; *s; s++) {
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
				}
				Data++;
				break;
			
			case 'd':										//十进制
				d = va_arg(ap, int);
				itoa(d, buf, 10);
				for (s = buf; *s; s++) {
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
				}
				Data++;
				break;
				
			default:
				Data++;
				break;
			}
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}


void USART1_put(char _ch) {
  USART_SendData(USART1, (u8) _ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


void USART1_puts(char _str[]) {
	for(int i = 0; _str[i] != 0; i++) {
		USART_SendData(USART1, _str[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}


void USART1_IRQHandler(void) {
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {    //若接收数据寄存器满
		LED_Change();
		if(RcvNext == RcvStart) {
			if(RcvStart == RcvBuf[RcvStart]) {
				RcvStart = ++RcvEnd;
				RcvBuf[RcvEnd] = RcvEnd;
			} else {
				for(u8 i = RcvStart; i < RcvBuf[RcvStart]; i++) RcvBuf[i] = i;
				RcvStart = RcvBuf[RcvStart];
			}
		}
		/**
		*此语句作用：将USART_DR寄存器的内容传到comm里。
		*另外,在单缓冲器模式下，软件读USART_DR寄存器则完成对RXNE位清除。
		*[注意]在多缓冲器模式下，必须通过软件清零"传输完成标志"DMA1_FLAG_TCx(即：令DMA_IFCR的位CTCIFx=1)，
		*否则将会无法跳出中断服务程序，出现一次中断请求无数次响应的后果
		**/
		RcvBuf[RcvNext] = USART_ReceiveData(USART1);
		RcvNext++;
		if(RcvBuf[RcvNext-2] == '\r' && RcvBuf[RcvNext-1] == '\n') {
			if(RcvNext == RcvEnd+3) RcvNext -= 2;
			else {
				RcvBuf[RcvEnd] = RcvNext-2;
				RcvEnd = RcvNext-2;
				RcvBuf[RcvEnd] = RcvEnd;
				RcvNext--;
				RcvBuf[RcvNext] = RcvNext;
			}
		}
		LED_Change();
	}
}


void RcvBufClear(void) {
	RcvStart = 0;
	RcvEnd = 0;
	RcvNext = 1;
	for(u8 i = 0; i != 0; i++) RcvBuf[i] = i;
}
