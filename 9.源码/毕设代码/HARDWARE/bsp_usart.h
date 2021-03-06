#ifndef __USART_H
#define	__USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"
#include <stdio.h>

/* 串口中断配置
 * 0：不开启中断
 * 1：开启接收中断、关闭发送中断
 * 2：开启发送中断，关闭接收中断
 * 3：开启发送和接收中断
*/
#define USART1_IRQ_ALLOW	0
#define USART3_IRQ_ALLOW	0

#define USART1_TXBUFF_SIZE   256
#define USART3_TXBUFF_SIZE   256


void UART1_Init(uint32_t baud);
void UART3_Init(uint32_t baud);
void u1_printf(char*, ...);         	//串口1 printf函数
void u3_printf(char*, ...);						//串口3 printf函数
#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
