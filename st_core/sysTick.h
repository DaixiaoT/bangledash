#ifndef _SYSTICK_H_
#define _SYSTICK_H_

extern volatile U32 g_sys_time;    // 当前系统时间, 单位: 毫秒. 硬件定时器中断更新

// 当前时间，毫秒
/*static inline U32 sys_time()
{
	return g_sys_time;
}*/

// 当前时间，毫秒, 返回值非0
static inline U32 sys_time()
{
    U32 t = g_sys_time;
    if(t)
        return t;
    return 1;
}

#define sys_time_nz sys_time

// 当前时间 到 t 的间隔 单位都是毫秒
static inline int TimeGap(U32 t) {
    return (int)(sys_time() - t);
}

void sysTick_init(void);

#ifndef _TOS_
void sleep(int ms);
#endif

#endif

