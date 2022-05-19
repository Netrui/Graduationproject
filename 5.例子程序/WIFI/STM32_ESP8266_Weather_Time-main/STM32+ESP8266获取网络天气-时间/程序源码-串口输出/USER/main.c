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

extern char Uart3_Buf[Buf3_Max];//串口2接收缓存

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
	delay_ms(1000);   //至少等待1s，必不可少
	CLR_Buf();   //清除数组
}


//字符串剪切函数
//str2：需要剪切的字符串
//str1：剪切后的字符串数组存放
//numxy：从第几个开始剪切
//lenth：要剪切的字符串长度
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

//系统硬件初始化
void Sys_Init(void)
{
	u8 i;
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	USART3_init(115200);
	LED_Init();		  	 //初始化与LED连接的硬件接口

	while(ESP8266_wifi_Init())    //检测WIFI模块是否插上
	{	
		printf("Init error\r\n");
		delay_ms(1000);
	}
	delay_ms(3000);
	ESP8266_Get_Net_Time();
	printf("GET http start\r\n");
}




//主函数
 int main(void)
 {	
	Sys_Init();
	delay_ms(1000);
	
	while(1)
	{
		CLR_Buf();   //清除数组
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


