#ifndef _UART2_H_
#define _UART2_H_
//#include "config.h"

void Uart2_init(U32 bps);

//int Uart2_TryWrite(const char *buf, int num);

// ֻ��Uart2_Write������������sema���������Զ��߳�ʹ��
void Uart2_Write(const char *buf, int num);

int Uart2_TryRead(char *buf, int num);
void Uart2_Read(char *buf, int num);

extern CommObj comm_uart2;

#define uart2_isr_lock()   __set_BASEPRI(((NVIC_UART2_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart2_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������uart2_isr_lock()��uart2_isr_unlock()֮�����===================
/*
// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
void Uart2_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int Uart2_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  Uart2_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int Uart2_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void Uart2_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int Uart2_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int Uart2_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart2_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart2_Search(int pos, const char *buf, int n);
*/

#endif //_UART2_H_

