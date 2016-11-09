 /******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
**********************************************************************************/
#include "UsartConf.h"
#include "LedConf.h"
#include <stdarg.h>

u8 RcvBuf[256] = {'0'};
u8 RcvStart = 0, RcvEnd = 0, RcvNext = 1;


/*******************************************************************************
	��������USART_Configuration
	��  ��:
	��  ��:
	����˵����
	��ʼ������Ӳ���豸�������ж�
	���ò��裺
	(1)��GPIO��USART��ʱ��
	(2)����USART�����ܽ�GPIOģʽ
	(3)����USART���ݸ�ʽ�������ʵȲ���
	(4)ʹ��USART�����жϹ���
	(5)���ʹ��USART����
*/
void USART1_Config(void) {
	LED_Config();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* ��4��������USART����
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

	/* ���������ݼĴ�������������ж� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART1, ENABLE);

	/* �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     // ���־
	
	//�����ʼ��
	for(u8 i = 0; i != 0; i++) RcvBuf[i] = i;
}

/* NVICȫ��ֻ����һ��
void NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
  
	Set the Vector Table base location at 0x08000000
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
	Configure the NVIC Preemption Priority Bits
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
	Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //���ж�
	NVIC_Init(&NVIC_InitStructure); 						   //��ʼ��
}
*/

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f) {
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	return (ch);
}


/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
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
 * ��������USART_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART_printf( USART1, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...) {
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0) {     // �ж��Ƿ񵽴��ַ���������
		if ( *Data == 0x5c ) {  //'\'
			switch ( *++Data ) {
			case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;
				
			case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;
				
			default:
				Data ++;
				break;
			}
		} else if ( *Data == '%') {
			switch ( *++Data ) {
			case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				for ( ; *s; s++) {
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
				}
				Data++;
				break;
			
			case 'd':										//ʮ����
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
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {    //���������ݼĴ�����
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
		*��������ã���USART_DR�Ĵ��������ݴ���comm�
		*����,�ڵ�������ģʽ�£������USART_DR�Ĵ�������ɶ�RXNEλ�����
		*[ע��]�ڶ໺����ģʽ�£�����ͨ���������"������ɱ�־"DMA1_FLAG_TCx(������DMA_IFCR��λCTCIFx=1)��
		*���򽫻��޷������жϷ�����򣬳���һ���ж�������������Ӧ�ĺ��
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
