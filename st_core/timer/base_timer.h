#ifndef _BASE_TIMER_H_
#define _BASE_TIMER_H_

struct BASE_TIEMR_CTL {
	//int ms_d10;
	TIMERPROC proc;
	U32 data;
	TIM_TypeDef *TIM;
	U8 prio;
};

#define timer_lock(n)   __set_BASEPRI(((g_timer[n].prio) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define timer_unlock() __set_BASEPRI(0)  // 0 表示不关闭任何中断

extern BASE_TIEMR_CTL g_timer[15];

// 单位: 微秒
static inline void Locked_SetTimer(int n, int us, TIMERPROC proc, U32 data)
{
	//g_timer4.ms_d10 = ms_d10;
	g_timer[n].proc = proc;
	g_timer[n].data = data;

	TIM_TypeDef *TIMx = g_timer[n].TIM;

    // Set the Autoreload value
    TIMx->ARR = us - 1;
    // reset counter
    TIMx->CNT = 0;
    /* Enable the Interrupt sources */
    //TIM4->DIER |= TIM_IT_Update;
    // Enable the TIM counter
    TIMx->CR1 |= TIM_CR1_CEN;
}

static inline void Locked_KillTimer(int n)
{
	g_timer[n].proc = 0;

    /* Disable the Interrupt sources */
    //TIM4->DIER &= (uint16_t)~TIM_IT_Update;
    /* Disable the TIM Counter */
    g_timer[n].TIM->CR1 &= (uint16_t)~TIM_CR1_CEN;
}

static inline void Locked_ResetTimer(int n)
{
    // reset counter
    g_timer[n].TIM->CNT = 0;
}

static inline void SetTimer(int n, int us, TIMERPROC proc, U32 data)
{
	timer_lock(n);

    Locked_SetTimer(n, us, proc, data);

	timer_unlock();
}

static inline void ResetTimer(int n)
{
	timer_lock(n);

    Locked_ResetTimer(n);

	timer_unlock();
}

static inline void KillTimer(int n)
{
	timer_lock(n);

    Locked_KillTimer(n);

	timer_unlock();
}

BOOL InitTimer(int n, U8 prio);

#endif //_BASE_TIMER_H_


