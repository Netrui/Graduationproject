#ifndef __WEATHER_H
#define __WEATHER_H

#include "sys.h"

typedef struct   //�ṹ�塣
{
    vu16  year;
    vu8   month;
    vu8   date;
    vu8   hour;
    vu8   min;
    vu8   sec;	 
}nt_calendar_obj;	

u8 get_current_weather(void);
u8 get_3days_weather(void);
void show_weather(void);

u8 get_beijing_time(void);
#endif


