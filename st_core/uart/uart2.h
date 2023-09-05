#ifndef _UART2_H_
#define _UART2_H_
//#include "config.h"

void Uart2_init(U32 bps);

//int Uart2_TryWrite(const char *buf, int num);

// 只有Uart2_Write是特例，加了sema保护，可以多线程使用
void Uart2_Write(const char *buf, int num);

int Uart2_TryRead(char *buf, int num);
void Uart2_Read(char *buf, int num);

extern CommObj comm_uart2;

#define uart2_isr_lock()   __set_BASEPRI(((NVIC_UART2_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart2_isr_unlock() __set_BASEPRI(0)  // 0 表示不关闭任何中断

// =============以下函数必须在uart2_isr_lock()和uart2_isr_unlock()之间调用===================
/*
// 从DMA buffer中刷新到uart rx_ringbuf中
void Uart2_RxFlush();

// 返回接收缓冲区中的有效字符数
int Uart2_RxNum();

// 从接收缓冲区peek一个字符. pos=0, 表示第一个有效字符
char  Uart2_PeekChar(int pos);

// 从接收缓冲区peek n个字符. pos=0, 表示第一个有效字符
// 返回有效的字符数
int Uart2_Peek(int pos, char *buf, int n);

// 接收缓冲区跳过n个字符
void Uart2_Skip(int n);

// 比较n个字符. =0, ok. <0, 错误.
int Uart2_ncmp(int pos, const char *buf, int n);

// 比较字符串. =0, ok. <0, 错误.
int Uart2_cmp(int pos, const char *str);

// 搜索1个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int Uart2_SearchChar(int pos, char c);

// 搜索n个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int Uart2_Search(int pos, const char *buf, int n);
*/

#endif //_UART2_H_

