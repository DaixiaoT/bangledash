#ifndef __RTC_ISL1208_H__
#define __RTC_ISL1208_H__

#define RTC_ADDR 0x6F    //时钟地址

#define OFF_SEC  0
#define OFF_MIN  1
#define OFF_HOUR 2
#define OFF_DAY  3
#define OFF_MON  4
#define OFF_YEAR 5
#define OFF_WDAY 6

#define TRUE 1
#define FALSE 0

//实时时钟数据结构
typedef struct
{
	U8 year;       //BCD码，2000开始
	U8 month;      //BCD码，1---12
	U8 day;        //BCD码，1---31
	U8 hour;       //BCD码，0---23
	U8 minute;     //BCD码，0---59
	U8 second;     //BCD码，0---59
//	U8 wday;       //BCD码，0---6   0--星期天
}Time_t;

void RTC_ISL1208_Init(Time_t *p);
BOOL RTC_ISL1208_SetTime(Time_t *p);
BOOL RTC_ISL1208_GetTime(Time_t *p,BOOL Force=FALSE);


static inline void RTC_ISL1208_print(Time_t *p)
{
	LOG_PRINT("RTC_ISL1208: year=%x mon=%x day=%x hour=%x min=%x second=%x \r\n",
			  p->year, p->month, p->day, p->hour, p->minute, p->second);
}


#endif //__RTC_H__

