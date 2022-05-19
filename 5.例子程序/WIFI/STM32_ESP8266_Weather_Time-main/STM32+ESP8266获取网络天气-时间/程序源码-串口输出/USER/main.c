#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wifi.h"
#include <string.h>
#include <stdlib.h>

char my_Date[35];
char my_Date1[35];

extern u8 flagy;

extern char Uart3_Buf[Buf3_Max];//����2���ջ���

char *str_shear(char *str1,char *str2,u8 numxy,u8 lenth);

struct Get_Time
{
	char My_week[4];
	char My_date[12];
	char My_time[8];
	char My_hour[3];
}Get_net_time;

void ESP8266_wifi_Init1(void)
{

	strncpy(my_Date,strstr(Uart3_Buf,"Date"),31);

	str_shear(my_Date,my_Date,6,25);
	
	str_shear(Get_net_time.My_week,my_Date,0,3);
	str_shear(Get_net_time.My_date,my_Date,5,11);
	str_shear(Get_net_time.My_time,my_Date,17,8);
	str_shear(Get_net_time.My_hour,Get_net_time.My_time,0,2);
	str_shear(Get_net_time.My_time,Get_net_time.My_time,2,6);
	

	printf("Date:  %s\r\n",Get_net_time.My_date);
	printf("Week:  %s\r\n",Get_net_time.My_week);
	printf("Time:  %d%s\r\n\r\n",atoi(Get_net_time.My_hour)+8,Get_net_time.My_time);
	delay_ms(1000);   //���ٵȴ�1s���ز�����
	CLR_Buf();   //�������
}


//�ַ������к���
//str2����Ҫ���е��ַ���
//str1�����к���ַ���������
//numxy���ӵڼ�����ʼ����
//lenth��Ҫ���е��ַ�������
char *str_shear(char *str1,char *str2,u8 numxy,u8 lenth)
{
	u8 i;
	
	for(i=numxy;i<numxy+lenth;i++)
	{
		str1[i-numxy] = str2[i];
	}
	str1[lenth] = '\0';
	return str1;
}

//ϵͳӲ����ʼ��
void Sys_Init(void)
{
	u8 i;
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	USART3_init(115200);
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�

	while(ESP8266_wifi_Init())    //���WIFIģ���Ƿ����
	{	
		printf("Init error\r\n");
		delay_ms(1000);
	}
	delay_ms(3000);
	ESP8266_Get_Net_Time();
	printf("GET http start\r\n");
}




//������
 int main(void)
 {	
	Sys_Init();
	delay_ms(1000);
	
	while(1)
	{
		CLR_Buf();   //�������
		if(flagy == 0)
		{
			
			UART3_SendString("GET http://cgi.im.qq.com");
			printf("Send http OK\r\n");
			delay_ms(1500);
			flagy = 1;
		}
		
		if(flagy == 1)
		{
			flagy = 0;
			printf("Uart3_Buf = %s\r\n",Uart3_Buf);
			//CLR_Buf(); 
			ESP8266_wifi_Init1();
			delay_ms(1500);
			ESP8266_Get_Net_Time();
		}
	}	 
}


