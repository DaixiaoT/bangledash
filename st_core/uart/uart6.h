#ifndef _UART6_H_
#define _UART6_H_
//#include "config.h"

void Uart6_init(U32 bps);
void Uart6_StopRevc();
void Uart6_StartRevc();

//int Uart6_TryWrite(const char *buf, int num);

// ֻ��Uart6_Write������������sema���������Զ��߳�ʹ��
void Uart6_Write(const char *buf, int num);

int Uart6_TryRead(char *buf, int num);
void Uart6_Read(char *buf, int num);

extern CommObj comm_uart6;

#define uart6_isr_lock()   __set_BASEPRI(((NVIC_UART6_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart6_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������uart6_isr_lock()��uart6_isr_unlock()֮�����===================
/*
// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
void Uart6_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int Uart6_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  Uart6_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int Uart6_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void Uart6_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int Uart6_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int Uart6_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart6_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart6_Search(int pos, const char *buf, int n);
*/

#endif //_UART6_H_

