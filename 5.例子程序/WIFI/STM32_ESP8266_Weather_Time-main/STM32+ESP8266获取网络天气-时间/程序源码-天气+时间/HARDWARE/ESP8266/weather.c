#include "weather.h"
#include "wifista.h"
#include "usart.h"
#include "malloc.h"
#include "usart.h"
#include "delay.h"
//#include "text.h"
#include "parsejson.h"
#include "key.h"
#include "led.h"
#include "string.h"
#include "rtc.h"
#include "24cxx.h"
#include "stdlib.h"

//�������Ӷ˿ں�:80	
#define WEATHER_PORTNUM 	"80"
//����������IP
#define WEATHER_SERVERIP 	"api.seniverse.com"

//ʱ��˿ں�
#define TIME_PORTNUM			"80"
//ʱ�������IP
#define TIME_SERVERIP			"cgi.im.qq.com"  

//����ṹ�����

 nt_calendar_obj nwt;  //����ṹ�����
u8 flag = 0;
//��ȡһ��ʵʱ����
//���أ�0---��ȡ�ɹ���1---��ȡʧ��
u8 get_current_weather(void)
{
	u8 *p = mymalloc(SRAMIN,40); //����40�ֽ��ڴ�
	u8 res;				
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",WEATHER_SERVERIP,WEATHER_PORTNUM);    //����Ŀ��TCP������
	res = atk_8266_send_cmd(p,"OK",200);//���ӵ�Ŀ��TCP������
	if(res==1)
	{
		myfree(SRAMIN,p);
		return 1;
	}
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMODE=1","OK",100);      //����ģʽΪ��͸��	
	atk_8266_send_cmd("AT+CIPSEND","OK",100);         //��ʼ͸��
	printf("start trans...\r\n");
	LED0 = ~LED0;
	if(LED0 == 0)
	{
		u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SWaCmu3LmzT_kS21g&location=changsha&language=zh-Hans&unit=c\n\n");
		flag = 1;
	}
	if(LED0 == 1)
	{
		u3_printf("GET https://api.seniverse.com/v3/weather/now.json?key=SWaCmu3LmzT_kS21g&location=hengyang&language=zh-Hans&unit=c\n\n");
		flag = 2;
	}
	
	delay_ms(10);//��ʱ10ms���ص���ָ��ͳɹ���״̬
	CLR_BUF();	//���㴮��3����
	delay_ms(1000);
	if(USART3_RX_STA != 0)		//��ʱ�ٴνӵ�һ�����ݣ�Ϊ����������
	{ 
		USART3_RX_BUF[USART3_RX_STA]=0;//��ӽ�����
	} 
	parse_now_weather();
	CLR_BUF();	//���㴮��3����
	
	atk_8266_quit_trans();//�˳�͸��
	atk_8266_send_cmd("AT+CIPCLOSE","OK",50);         //�ر�����
	myfree(SRAMIN,p);
	return 0;
}

extern u16 count;

//��ȡ3�������
u8 get_3days_weather(void)
{
	u8 res;
	u8 *p = mymalloc(SRAMIN,40); //����40�ֽ��ڴ�							
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",WEATHER_SERVERIP,WEATHER_PORTNUM);    //����Ŀ��TCP������
	res = atk_8266_send_cmd(p,"OK",200);//���ӵ�Ŀ��TCP������
	if(res==1)
	{
		myfree(SRAMIN,p);
		return 1;
	}
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMODE=1","OK",100);      //����ģʽΪ��͸��	
	CLR_BUF();
	atk_8266_send_cmd("AT+CIPSEND","OK",100);         //��ʼ͸��
	printf("start trans...\r\n");
	CLR_BUF();
    //�ҵ���Կ SWaCmu3LmzT_kS21g
	if(flag == 1)
		u3_printf("GET https://api.seniverse.com/v3/weather/daily.json?key=SWaCmu3LmzT_kS21g&location=changsha&language=zh-Hans&unit=c&start=0&days=5\n\n");
	if(flag == 2)
		u3_printf("GET https://api.seniverse.com/v3/weather/daily.json?key=SWaCmu3LmzT_kS21g&location=hengyang&language=zh-Hans&unit=c&start=0&days=5\n\n");
	delay_ms(10);//��ʱ10ms���ص���ָ��ͳɹ���״̬
	CLR_BUF();
	delay_ms(1500);
	if(USART3_RX_STA != 0)
		USART3_RX_BUF[USART3_RX_STA]=0;//��ӽ�����
	parse_3days_weather();
	CLR_BUF();
	atk_8266_quit_trans();//�˳�͸��
	atk_8266_send_cmd("AT+CIPCLOSE","OK",50);         //�ر�����
	myfree(SRAMIN,p);
	return 0;
}

//��ȡ����ʱ��
u8 get_beijing_time(void)
{
	u8 *p;
	u8 res;
	
	u8 *resp;
	u8 *p_end;
//	u8 ipbuf[16]; 	//IP����
	p=mymalloc(SRAMIN,40);							//����40�ֽ��ڴ�
	resp=mymalloc(SRAMIN,10);
	p_end=mymalloc(SRAMIN,40);
	
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",TIME_SERVERIP,TIME_PORTNUM);    //����Ŀ��TCP������
	res = atk_8266_send_cmd(p,"OK",200);//���ӵ�Ŀ��TCP������
	if(res==1)
	{
		myfree(SRAMIN,p);
		return 1;
	}
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMODE=1","OK",100);      //����ģʽΪ��͸��	

	printf("�豸 %s\r\n",p);
	
	CLR_BUF();
	atk_8266_send_cmd("AT+CIPSEND","OK",100);         //��ʼ͸��
	printf("start trans...\r\n");

	u3_printf("GET http://cgi.im.qq.com\n\n");
	delay_ms(20);
	CLR_BUF();	
	delay_ms(1000);

	if(USART3_RX_STA != 0)
    {
		strncpy((char *)resp,"Date",5);
        USART3_RX_BUF[USART3_RX_STA] = 0;
		//printf("get_tim_srt��%s\r\n",USART3_RX_BUF);
        if(strstr((char*)USART3_RX_BUF,(char*)resp)) 
        {       
			strncpy((char *)resp,"GMT",4);
            p_end = (u8*)strstr((char*)USART3_RX_BUF,(char*)resp);
			p = p_end - 9; 
			//printf("get_net_str %s\r\n",p);
			nwt.hour = ((*p - 0x30)*10 + (*(p+1) - 0x30) + 8) % 24;  //GMT0-->GMT8

			nwt.min = ((*(p+3) - 0x30)*10 + (*(p+4) - 0x30)) % 60;

			nwt.sec = ((*(p+6) - 0x30)*10 + (*(p+7) - 0x30)) % 60;

			nwt.year = ((*(p-5) - 0x30)*1000 + (*(p-4) - 0x30)*100+ (*(p-3) - 0x30)*10+ (*(p-2) - 0x30)); 

			nwt.date = ((*(p-12) - 0x30)*10 + (*(p-11) - 0x30)); 

			if        ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Jan")) nwt.month=1; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Feb")) nwt.month=2; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Mar")) nwt.month=3; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Apr")) nwt.month=4; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "May")) nwt.month=5; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Jun")) nwt.month=6; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Jul")) nwt.month=7; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Aug")) nwt.month=8; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Sep")) nwt.month=9; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Oct")) nwt.month=10; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Nov")) nwt.month=11; 
			else if   ((u8*)strstr((char*)USART3_RX_BUF,(char*) "Dec")) nwt.month=12;
 
			CLR_BUF();
												 
			printf("uddate:nettime!!!");
			RTC_Set(nwt.year,nwt.month ,nwt.date ,nwt.hour ,nwt.min,nwt.sec);											
														
	    }
		CLR_BUF();														
    }               
	atk_8266_quit_trans();//�˳�͸��
	atk_8266_send_cmd("AT+CIPCLOSE","OK",50);         //�ر�����
	myfree(SRAMIN,p);
	myfree(SRAMIN,resp);
	myfree(SRAMIN,p_end);
	return 0;
}
