#ifndef _UART0_H_
#define _UART0_H_
//#include "config.h"

void Uart0_init(U32 bps);

//int Uart0_TryWrite(const char *buf, int num);

// ֻ��Uart0_Write������������sema���������Զ��߳�ʹ��
void Uart0_Write(const char *buf, int num);

int Uart0_TryRead(char *buf, int num);
void Uart0_Read(char *buf, int num);

int buf_print(char *buf, int buf_len, const char *fmt, ... );

void LOG_PRINT(const char *fmt, ... );

extern CommObj comm_uart0;

#define uart0_isr_lock()   __set_BASEPRI(((NVIC_UART0_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart0_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������uart0_isr_lock()��uart0_isr_unlock()֮�����===================

// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
void Uart0_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int Uart0_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  Uart0_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int Uart0_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void Uart0_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int Uart0_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int Uart0_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart0_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart0_Search(int pos, const char *buf, int n);

#endif //_UART0_H_

