#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "rtc.h"
#include "24cxx.h"
#include "wdg.h"
#include "cJSON.h" 
#include "wifista.h"
#include "weather.h"
#include "malloc.h"

extern u8 flag;				//�����л������������õĺ����ͳ�ɳ�������е�����
extern nt_calendar_obj nwt;  //����ṹ�����

void show_weather(void);
void show_time(void);
	
 int main(void)
 {	 
	u8 min;	 
	u16 t = 0;
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	usart3_init(115200);		//��ʼ������3 
	LED_Init();			     	//LED�˿ڳ�ʼ��
	LCD_Init();           		//LCD��ʼ��
	LCD_Clear(BLACK);    		//�������Ժ�ɫ����
	BACK_COLOR = BLACK;  		//��ɫ����
//	IWDG_Init(5,4000);
	RTC_Init();           		//RTCʱ�ӳ�ʼ��
	KEY_Init();         		//������ʼ��
	AT24CXX_Init();             //24C02��ʼ��
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	POINT_COLOR=RED;            //��ɫ
	delay_ms(200);              //��LCD ID��ӡ��lcd_id���顣				 	
	atk_8266_quit_trans();      //�˳�͸��
	atk_8266_wifista_config();  //��ʼ��WiFi�����Ӷ�Ӧ·��������������
	printf("wifi init ok!");  
	POINT_COLOR = RED;    		//��ʾ��ɫ
	delay_ms(1000); 
	delay_ms(1000);      		//�ʵ���ʱ
	show_weather();				//��ʾ�����ʾ��
	get_current_weather();		//��ȡ��ǰ�������
	delay_ms(200);			
	get_3days_weather();		//��ȡδ��������������
	delay_ms(200);
	get_beijing_time();			//��ȡ����ʱ��
	delay_ms(200);

	RTC_Set(nwt.year,nwt.month,nwt.date,nwt.hour,nwt.min,nwt.sec);  //����ʱ��	
	min = calendar.min;
	while(1) 
	{		 
		delay_ms(10);
		t++;
		if(t==1000)
		{
			if(atk_8266_consta_check())//�õ�����״̬
			{
				printf("link normal!\r\n");
			}				
			else
			{
				printf("connect error,trying connect again......\r\n");
				atk_8266_wifista_config();
			}
			t=0;
		}
		
		if(min!=calendar.min)  //ÿ��һ���Ӹ���һ���������
		{
			
			get_current_weather();	  //��ǰ�������
			
			get_3days_weather();	  //δ�������������		
			min = calendar.min; 	
			
		}
	} 
}
 

void show_weather(void)
{
    POINT_COLOR = YELLOW;
	LCD_ShowString(0,0,sizeof("0000-00-00")*8,16,16,"0000-00-00");  //����
	LCD_ShowString(88,0,sizeof("00:00:00")*8,16,16,"00:00:00");		//ʱ��
	LCD_ShowString(100,20,200,20,16,(u8*)"Last Update:");			//���һ����������ʱ��
	LCD_ShowString(0,40,sizeof("weather:")*8,16,16,"weather:");		//����
	POINT_COLOR = YELLOW;
	LCD_ShowString(0,60,sizeof("wind_dir & grade & speed:")*8,16,16,"wind_dir & grade & speed:"); //���򡢷��ٵȼ�������
	LCD_ShowString(0,100,sizeof("temp & humi:")*8,16,16,"temp & humi:");  //��ʪ��
	
	POINT_COLOR = BLUE;
	LCD_ShowString(100,140,200,20,16,(u8*)"Last Update:");
	LCD_ShowString(0,160,sizeof("weather:")*8,16,16,"weather:");
	POINT_COLOR = BLUE;
	LCD_ShowString(0,180,sizeof("wind_dir & grade & speed:")*8,16,16,"wind_dir & grade & speed:");
	LCD_ShowString(0,220,sizeof("temp & humi:")*8,16,16,"temp & humi:");
}

//RTC ʱ����ʾ
void show_time(void)
{
	POINT_COLOR = RED;
	LCD_ShowNum(0,0,calendar.w_year,4,16);
	LCD_ShowxNum(40,0,calendar.w_month,2,16,0x80);
	LCD_ShowxNum(64,0,calendar.w_date,2,16,0x80);	
	LCD_ShowxNum(88,0,calendar.hour,2,16,0x80);
	LCD_ShowxNum(112,0,calendar.min,2,16,0x80);	
	LCD_ShowxNum(136,0,calendar.sec,2,16,0x80);	
}

