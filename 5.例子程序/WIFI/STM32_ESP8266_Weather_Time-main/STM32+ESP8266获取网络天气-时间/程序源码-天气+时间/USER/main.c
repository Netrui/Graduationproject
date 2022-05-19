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

extern u8 flag;				//城市切换变量，例程用的衡阳和长沙两个城市的天气
extern nt_calendar_obj nwt;  //定义结构体变量

void show_weather(void);
void show_time(void);
	
 int main(void)
 {	 
	u8 min;	 
	u16 t = 0;
	delay_init();	    	 	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
	usart3_init(115200);		//初始化串口3 
	LED_Init();			     	//LED端口初始化
	LCD_Init();           		//LCD初始化
	LCD_Clear(BLACK);    		//清屏，以黑色清屏
	BACK_COLOR = BLACK;  		//黑色字体
//	IWDG_Init(5,4000);
	RTC_Init();           		//RTC时钟初始化
	KEY_Init();         		//按键初始化
	AT24CXX_Init();             //24C02初始化
	my_mem_init(SRAMIN);		//初始化内部内存池
	POINT_COLOR=RED;            //红色
	delay_ms(200);              //将LCD ID打印到lcd_id数组。				 	
	atk_8266_quit_trans();      //退出透传
	atk_8266_wifista_config();  //初始化WiFi，连接对应路由器，连接网络
	printf("wifi init ok!");  
	POINT_COLOR = RED;    		//显示红色
	delay_ms(1000); 
	delay_ms(1000);      		//适当延时
	show_weather();				//显示相关提示词
	get_current_weather();		//获取当前天气情况
	delay_ms(200);			
	get_3days_weather();		//获取未来三天的天气情况
	delay_ms(200);
	get_beijing_time();			//获取网络时间
	delay_ms(200);

	RTC_Set(nwt.year,nwt.month,nwt.date,nwt.hour,nwt.min,nwt.sec);  //设置时间	
	min = calendar.min;
	while(1) 
	{		 
		delay_ms(10);
		t++;
		if(t==1000)
		{
			if(atk_8266_consta_check())//得到连接状态
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
		
		if(min!=calendar.min)  //每过一分钟更新一下天气情况
		{
			
			get_current_weather();	  //当前天气情况
			
			get_3days_weather();	  //未来三天天气情况		
			min = calendar.min; 	
			
		}
	} 
}
 

void show_weather(void)
{
    POINT_COLOR = YELLOW;
	LCD_ShowString(0,0,sizeof("0000-00-00")*8,16,16,"0000-00-00");  //日期
	LCD_ShowString(88,0,sizeof("00:00:00")*8,16,16,"00:00:00");		//时间
	LCD_ShowString(100,20,200,20,16,(u8*)"Last Update:");			//最近一次天气更新时间
	LCD_ShowString(0,40,sizeof("weather:")*8,16,16,"weather:");		//天气
	POINT_COLOR = YELLOW;
	LCD_ShowString(0,60,sizeof("wind_dir & grade & speed:")*8,16,16,"wind_dir & grade & speed:"); //风向、风速等级、风速
	LCD_ShowString(0,100,sizeof("temp & humi:")*8,16,16,"temp & humi:");  //温湿度
	
	POINT_COLOR = BLUE;
	LCD_ShowString(100,140,200,20,16,(u8*)"Last Update:");
	LCD_ShowString(0,160,sizeof("weather:")*8,16,16,"weather:");
	POINT_COLOR = BLUE;
	LCD_ShowString(0,180,sizeof("wind_dir & grade & speed:")*8,16,16,"wind_dir & grade & speed:");
	LCD_ShowString(0,220,sizeof("temp & humi:")*8,16,16,"temp & humi:");
}

//RTC 时钟显示
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

