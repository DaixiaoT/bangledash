#ifndef _SYS_API_H_
#define _SYS_API_H_

#include "stm32f4xx.h"

void CheckUart0Bus();
#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

EXTERN_C_BEGIN

//这一段无需改动
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
	#define NULL 0
#endif

#define my_max(a,b)            (((a) > (b)) ? (a) : (b))
#define my_min(a,b)            (((a) < (b)) ? (a) : (b))

#define __ALIGN4__ __attribute__((aligned(4)))
#define __ALIGN2__ __attribute__((aligned(2)))

typedef signed   char  S8;
typedef          short S16;
typedef          long  S32;
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned long  U32;
typedef          long long S64;
typedef unsigned long long U64;

typedef signed char I8;
typedef signed short I16;

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;

typedef float          fp32;                    /* 单精度浮点数（32位长度）                 */
typedef double         fp64;                    /* 双精度浮点数（64位长度）                 */

typedef int	BOOL;




#define VPshort		*(volatile unsigned short *)

#define INLINE inline

static inline U16 my_swap16(U16 v)
{
	return (v >> 8) | (v << 8);
}

static inline U32 my_swap32(U32 v)
{
	return ((v >> 24) & 0x000000FF) |
		   ((v >> 8)  & 0x0000FF00) |
		   ((v << 8)  & 0x00FF0000) |
		   ((v << 24) & 0xFF000000);
}

static inline U32 my_swap64(U32 ll)
{
	return (((ll) >> 56) | 
		(((ll)& 0x00ff000000000000) >> 40) | 
		(((ll)& 0x0000ff0000000000) >> 24) | 
		(((ll)& 0x000000ff00000000) >> 8) | 
		(((ll)& 0x00000000ff000000) << 8) | 
		(((ll)& 0x0000000000ff0000) << 24) | 
		(((ll)& 0x000000000000ff00) << 40) | 
		(((ll) << 56)));
}

static inline void mem_move(U8 *dst, const U8 *src, int len)
{
    if(src < dst) {
        src += len-1;
        dst += len-1;
        while(len>0) {
            *dst-- = *src--;
            len--;
        }
    }
    else {
        while(len>0) {
            *dst++ = *src++;
            len--;
        }
    }
}

static inline void mem_move32(U32 *dst, const U32 *src, int len32)
{
    if(src < dst) {
        src += len32-1;
        dst += len32-1;
        while(len32>0) {
            *dst-- = *src--;
            len32--;
        }
    }
    else {
        while(len32>0) {
            *dst++ = *src++;
            len32--;
        }
    }
}

static inline void mem_copy(U8 *dst, const U8 *src, int len)
{
    while(len>0) {
        *dst++ = *src++;
        len--;
    }
}
//#define memcpy(dst,src,len) mem_copy((U8*)dst,(U8*)src,len)

static inline void mem_copy32(U32 *dst, const U32 *src, int len32)
{
    while(len32>0) {
        *dst++ = *src++;
        len32--;
    }
}

static inline void mem_set(U8 *dst, U8 c, int len)
{
    while(len>0) {
        *dst++ = c;
        len--;
    }
}
//#define memset(dst,c,len) mem_set((U8*)dst, c, len)

static inline void mem_set32(U32 *dst, U32 c, int len32)
{
    while(len32>0) {
        *dst++ = c;
        len32--;
    }
}

// return 0, 两块内存完全相同，1，不同
static inline int mem_cmp(U8 *dst, const U8 *src, int len)
{
    while(len>0) {
        if(*dst++ != *src++)
            return 1;
        len--;
    }
    return 0;
}

//#define memcmp(dst,src,len) mem_cmp((U8*)dst,(U8*)src,len)
// return 0, 两块内存完全相同，1，不同
static inline int mem_cmp32(U32 *dst, const U32 *src, int len32)
{
    while(len32>0) {
        if(*dst++ != *src++)
            return 1;
        len32--;
    }
    return 0;
}

#define hton16(x) my_swap16(x)
#define ntoh16(x) my_swap16(x)
#define hton32(x) my_swap32(x)
#define ntoh32(x) my_swap32(x)
#define htons(x) hton16(x)
#define ntohs(x) ntoh16(x)
#define htonl(x) hton32(x)
#define ntohl(x) ntoh32(x)

// 定时器回调函数原型
// 返回1：表示定时器自动重复调用
// 返回0：表示定时器只调用一次
typedef int (*TIMERPROC)(U32 data);

// 请求调度，通过设置ICSR中的PendSV异常来实现，在sched_unlock()时会立即进入异常
#define SetPendSV() SCB->ICSR = SCB_ICSR_PENDSVSET_Msk

#include "tools.h"

#include "gpio.h"

#include "sys_config.h" 

#include "sysTick.h"

#include "mi_mem.h"

#include "ringbuf.h"
#include "comm_obj.h"

#include "string1.h"

#include "uart0.h"

#include "rtc_boot_flag.h"

#ifdef _TOS_
#include "tos.h"
#endif

#ifdef _UART6_
#include "uart6.h"
#endif

#ifdef _UART2_
#include "uart2.h"
#endif

#ifdef _UART3_
#include "uart3.h"
#endif

#ifdef _UART4_
#include "uart4.h"
#endif

#ifdef _USB_
#include "usb_uart.h"
#endif

//#ifdef _CAN_
#include "can.h"
#include "can_id.h"
//#endif

#ifdef _I2C0_
#include "i2c0.h"
#endif

#ifdef _I2C1_
#include "i2c1.h"
#endif

#ifdef _I2C2_
#include "i2c2.h"
#endif

#ifdef _RTC_
#include "rtc.h"
#endif

#ifdef _DMA_
#include "dma.h" // gpdma driver
#endif

#ifdef _SD_
//#include "sd.h" // sd card driver
#endif

#ifdef _EMC_
//void *gdi_alloc(int size);
#endif


int sys_init();

// interrupt priority

#define NVIC_PWM_PRIO     9

#define NVIC_CAN_PRIO     3
#define NVIC_CAN_EX_PRIO  4

#define NVIC_USB_FS_PRIO  8

#define NVIC_EXINT_PRIO   6

#define NVIC_UART0_PRIO   5
// UART1~4, 可能需要使用timer2提供的定时功能(485使能)，所以中断优先级必须与timer1相同
#define NVIC_UART6_PRIO   10
#define NVIC_UART2_PRIO   10
#define NVIC_UART3_PRIO   10
#define NVIC_UART4_PRIO   10

#define NVIC_DMA_PRIO 8
#define NVIC_MCI_PRIO 8

// I2C，可能需要使用timer0提供的定时功能，所以中断优先级必须与timer0相同
#define NVIC_I2C1_PRIO   3
#define NVIC_I2C2_PRIO   3
#define NVIC_I2C3_PRIO   3

#define NVIC_SYSTICK_PRIO  14   // sysTick, 优先级次低，仅仅高于PendSV
#define NVIC_PendSV_PRIO   15   // 最低优先级

#define STACK_MARK     0xA55AF601

int main();

#define disable_interrupt() __disable_irq()
#define enable_interrupt() __enable_irq()

#define ALIGN4 __attribute__ ((aligned (4)))
//#define IN_IRAM __attribute__ ((section(".iram")))
//#define IN_IRAM4 __attribute__ ((section(".iram"))) __attribute__ ((aligned (4)))
#define IN_IRAM4 ALIGN4

//#include "iap.h"
//#include "update.h"

void memdump(unsigned char *buf, int len);
extern "C" int my_sprintf(char *buf, int buf_len, const char *fmt, ...);
#define mi_printf LOG_PRINT


#if 1 //def MI_ASSERT_EN
#define mi_assert(n)  { if((n)==0) { mi_printf("assert failed %s line %d (%d)\r\n", __FILE__, __LINE__,(n)); while(1) sleep(1000); }}
#else
#define mi_assert(n)
#endif // _DEBUG_

#include "base_timer.h"

#include "sys_log.h"

EXTERN_C_END

#endif

