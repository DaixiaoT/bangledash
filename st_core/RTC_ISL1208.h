#ifndef __RTC_ISL1208_H__
#define __RTC_ISL1208_H__

#define RTC_ADDR 0x6F    //ʱ�ӵ�ַ

#define OFF_SEC  0
#define OFF_MIN  1
#define OFF_HOUR 2
#define OFF_DAY  3
#define OFF_MON  4
#define OFF_YEAR 5
#define OFF_WDAY 6

#define TRUE 1
#define FALSE 0

//ʵʱʱ�����ݽṹ
typedef struct
{
	U8 year;       //BCD�룬2000��ʼ
	U8 month;      //BCD�룬1---12
	U8 day;        //BCD�룬1---31
	U8 hour;       //BCD�룬0---23
	U8 minute;     //BCD�룬0---59
	U8 second;     //BCD�룬0---59
//	U8 wday;       //BCD�룬0---6   0--������
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

