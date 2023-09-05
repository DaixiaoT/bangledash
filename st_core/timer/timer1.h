#ifndef _TIMER1_H_
#define _TIMER1_H_

#define timer1_lock()   __set_BASEPRI(((NVIC_TIMER1_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define timer1_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// ����������ֻ������TIMER0ͬһ�ж����ȼ���ISR�е���
// void Locked_SetTimer1(int slot, int ms_d10, TIMERPROC proc, U32 data);
// void Locked_ResetTimer1(int slot);
// void Locked_KillTimer1(int slot);
// 
// void SetTimer1(int slot, int ms_d10, TIMERPROC proc, U32 data);
// void ResetTimer1(int slot);
// void KillTimer1(int slot);

void InitTimer1();

#endif

