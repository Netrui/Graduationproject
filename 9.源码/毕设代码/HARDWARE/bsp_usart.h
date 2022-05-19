#ifndef __USART_H
#define	__USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include <stdio.h>

/* �����ж�����
 * 0���������ж�
 * 1�����������жϡ��رշ����ж�
 * 2�����������жϣ��رս����ж�
 * 3���������ͺͽ����ж�
*/
#define USART1_IRQ_ALLOW	0
#define USART3_IRQ_ALLOW	0

#define USART1_TXBUFF_SIZE   256
#define USART3_TXBUFF_SIZE   256


void UART1_Init(uint32_t baud);
void UART3_Init(uint32_t baud);
void u1_printf(char*, ...);         	//����1 printf����
void u3_printf(char*, ...);						//����3 printf����
#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
