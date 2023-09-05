#ifndef __RTC_H__
#define __RTC_H__

#ifdef _RTC_  
//实时时钟数据结构
typedef struct
{
	U8 year;       //BCD码，2000开始
	U8 month;      //BCD码，1---12
	U8 day;        //BCD码，1---31
	U8 hour;       //BCD码，0---23
	U8 minute;     //BCD码，0---59
	U8 second;     //BCD码，0---59
	U8 wday;       //BCD码，0---6   0--星期天
}Time_t;

void RTC_Init(Time_t *p);
BOOL RTC_SetTime(Time_t *p);
BOOL RTC_GetTime(Time_t *p);
void RTC_Time_Copy(Time_t *p,Time_t *time);

static inline void RTC_print(Time_t *p)
{
	LOG_PRINT("RTC: year=%x mon=%x day=%x hour=%x min=%x second=%x \r\n",
			  p->year, p->month, p->day, p->hour, p->minute, p->second);
}

#endif//_RTC_


#ifdef _RTC_1208_ 
#include "RTC_ISL1208.h"
#define RTC_Init	RTC_ISL1208_Init
#define RTC_SetTime RTC_ISL1208_SetTime
#define RTC_GetTime RTC_ISL1208_GetTime

static inline void RTC_print(Time_t *p)
{
	LOG_PRINT("RTC: year=%x mon=%x day=%x hour=%x min=%x second=%x wday=%x\r\n",
			  p->year, p->month, p->day, p->hour, p->minute, p->second);
}
#endif//_RTC_1208_



#endif //__RTC_H__




