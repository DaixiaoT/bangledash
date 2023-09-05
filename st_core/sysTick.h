#ifndef _SYSTICK_H_
#define _SYSTICK_H_

extern volatile U32 g_sys_time;    // ��ǰϵͳʱ��, ��λ: ����. Ӳ����ʱ���жϸ���

// ��ǰʱ�䣬����
/*static inline U32 sys_time()
{
	return g_sys_time;
}*/

// ��ǰʱ�䣬����, ����ֵ��0
static inline U32 sys_time()
{
    U32 t = g_sys_time;
    if(t)
        return t;
    return 1;
}

#define sys_time_nz sys_time

// ��ǰʱ�� �� t �ļ�� ��λ���Ǻ���
static inline int TimeGap(U32 t) {
    return (int)(sys_time() - t);
}

void sysTick_init(void);

#ifndef _TOS_
void sleep(int ms);
#endif

#endif

