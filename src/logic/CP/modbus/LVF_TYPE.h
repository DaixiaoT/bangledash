#ifndef __LVF_TYPES_H__
#define __LVF_TYPES_H__

typedef U8    INT8U;
typedef U16   INT16U;
typedef U32   INT32U;
typedef I8  INT8S;
typedef I16 INT16S;
typedef int INT32S;

//typedef U16 BIT_WORD;


typedef U32 rt_tick_t;
typedef BOOL rt_bool_t;


#define rt_thread_delay sleep
#define rt_kprintf	LOG_PRINT
#define memcpy mem_copy

#endif//__LVF_TYPES_H__


