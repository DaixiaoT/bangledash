#ifndef _UART3_H_
#define _UART3_H_
//#include "config.h"

void Uart3_init(U32 bps);
void Uart3_StopRevc();
void Uart3_StartRevc();

//int Uart3_TryWrite(const char *buf, int num);

// 只有Uart3_Write是特例，加了sema保护，可以多线程使用
void Uart3_Write(const char *buf, int num);

int Uart3_TryRead(char *buf, int num);
void Uart3_Read(char *buf, int num);

extern CommObj comm_uart3;

#define uart3_isr_lock()   __set_BASEPRI(((NVIC_UART3_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart3_isr_unlock() __set_BASEPRI(0)  // 0 表示不关闭任何中断

// =============以下函数必须在uart3_isr_lock()和uart3_isr_unlock()之间调用===================
/*
// 从DMA buffer中刷新到uart rx_ringbuf中
void Uart3_RxFlush();

// 返回接收缓冲区中的有效字符数
int Uart3_RxNum();

// 从接收缓冲区peek一个字符. pos=0, 表示第一个有效字符
char  Uart3_PeekChar(int pos);

// 从接收缓冲区peek n个字符. pos=0, 表示第一个有效字符
// 返回有效的字符数
int Uart3_Peek(int pos, char *buf, int n);

// 接收缓冲区跳过n个字符
void Uart3_Skip(int n);

// 比较n个字符. =0, ok. <0, 错误.
int Uart3_ncmp(int pos, const char *buf, int n);

// 比较字符串. =0, ok. <0, 错误.
int Uart3_cmp(int pos, const char *str);

// 搜索1个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int Uart3_SearchChar(int pos, char c);

// 搜索n个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int Uart3_Search(int pos, const char *buf, int n);
*/

#endif //_UART3_H_

