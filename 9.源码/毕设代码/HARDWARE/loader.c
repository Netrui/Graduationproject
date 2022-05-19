#include "loader.h"
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include "bsp_usart.h"

void uart_send_instruction(char *instruction)
{
	while (*instruction !=0) {
		USART3->DR = *instruction++;
		while((USART3->SR&0X40)==0);//循环发送,直到发送完毕
	}
}

/* 开启wifi，包括打开wifi，设置wifi账号密码，开启连接 */
void Wifi_Start()
{
	uart_send_instruction(AT_R);
	delay_ms(200);
	uart_send_instruction(AT_B);
	delay_ms(200);
	uart_send_instruction(AT_CWMODE_B);
	delay_ms(200);
	uart_send_instruction(AT_CWSAP_DEF_B);
	delay_ms(200);
	uart_send_instruction(AT_CIFSR_B);
	delay_ms(200);
	uart_send_instruction(AT_CIPMUX_B);
	delay_ms(200);
	uart_send_instruction(AT_CIPSERVER_B);
	delay_ms(500);
}

void Wifi_SendBuffer(char *buf, uint16_t size)
{
	u3_printf("AT+CIPSEND=0,%d\r\n", size);
	delay_ms(100);
	u3_printf(buf);
}

void Wifi_SendString(char *str)
{
	u3_printf("AT+CIPSEND=0,%d\r\n", strlen(str));
	delay_ms(100);
	u3_printf(str);
}
