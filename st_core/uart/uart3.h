#ifndef _UART3_H_
#define _UART3_H_
//#include "config.h"

void Uart3_init(U32 bps);
void Uart3_StopRevc();
void Uart3_StartRevc();

//int Uart3_TryWrite(const char *buf, int num);

// ֻ��Uart3_Write������������sema���������Զ��߳�ʹ��
void Uart3_Write(const char *buf, int num);

int Uart3_TryRead(char *buf, int num);
void Uart3_Read(char *buf, int num);

extern CommObj comm_uart3;

#define uart3_isr_lock()   __set_BASEPRI(((NVIC_UART3_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart3_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������uart3_isr_lock()��uart3_isr_unlock()֮�����===================
/*
// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
void Uart3_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int Uart3_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  Uart3_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int Uart3_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void Uart3_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int Uart3_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int Uart3_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart3_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart3_Search(int pos, const char *buf, int n);
*/

#endif //_UART3_H_

