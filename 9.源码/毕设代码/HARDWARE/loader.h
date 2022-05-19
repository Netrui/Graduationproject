#ifndef LOADER_H
#define LOADER_H

#include "sys.h"

// ESP AT÷∏¡Ó
#define AT_R "AT+RESTORE\r\n"
#define AT_B			"AT\r\n"
#define AT_CWMODE_B		"AT+CWMODE=2\r\n"
#define AT_CWSAP_DEF_B	"AT+CWSAP_DEF=\"esp8266\",\"12345678\",5,4\r\n"
#define AT_CIFSR_B		"AT+CIFSR\r\n"
#define AT_CIPMUX_B		"AT+CIPMUX=1\r\n"
#define AT_CIPSERVER_B	"AT+CIPSERVER=1,8080\r\n"
#define AT_SEND_B		"AT+CIPSEND=0,"
#define AT_END_B		"\r\n"
#define AT_CLOSE_B		"AT+CIPSERVER=0\r\n"

void Wifi_Start(void);
void Wifi_SendBuffer(char *buf, uint16_t size);
void Wifi_SendString(char *str);

#endif
