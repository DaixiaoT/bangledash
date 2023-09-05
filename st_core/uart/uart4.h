#ifndef _UART4_H_
#define _UART4_H_
//#include "config.h"

void Uart4_init(U32 bps);
void Uart4_StopRevc();
void Uart4_StartRevc();

//int Uart4_TryWrite(const char *buf, int num);

// ֻ��Uart4_Write������������sema���������Զ��߳�ʹ��
void Uart4_Write(const char *buf, int num);

int Uart4_TryRead(char *buf, int num);
void Uart4_Read(char *buf, int num);

extern CommObj comm_uart4;

#define uart4_isr_lock()   __set_BASEPRI(((NVIC_UART4_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define uart4_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������uart4_isr_lock()��uart4_isr_unlock()֮�����===================
/*
// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
void Uart4_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int Uart4_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  Uart4_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int Uart4_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void Uart4_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int Uart4_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int Uart4_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart4_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int Uart4_Search(int pos, const char *buf, int n);
*/

#endif //_UART4_H_

