#ifndef _TIMER2_H_
#define _TIMER2_H_

#define timer2_lock()   __set_BASEPRI(((NVIC_TIMER2_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define timer2_unlock() __set_BASEPRI(0)  // 0 表示不关闭任何中断

extern BASE_TIEMR_CTL g_timer2;

// 单位: 微秒
static inline void Locked_SetTimer2(int us, TIMERPROC proc, U32 data)
{
	//g_timer2.ms_d10 = ms_d10;
	g_timer2.proc = proc;
	g_timer2.data = data;

    // Set the Autoreload value
    TIM2->ARR = us - 1;
    // reset counter
    TIM2->CNT = 0;
    /* Enable the Interrupt sources */
    //TIM2->DIER |= TIM_IT_Update;
    // Enable the TIM counter
    TIM2->CR1 |= TIM_CR1_CEN;
}

static inline void Locked_KillTimer2()
{
	//g_timer2.ms_d10 = 0;
	g_timer2.proc = 0;

    /* Disable the Interrupt sources */
    //TIM2->DIER &= (uint16_t)~TIM_IT_Update;
    /* Disable the TIM Counter */
    TIM2->CR1 &= (uint16_t)~TIM_CR1_CEN;
}

static inline void Locked_ResetTimer2()
{
    // reset counter
    TIM2->CNT = 0;
}

static inline void SetTimer2(int ms_d10, TIMERPROC proc, U32 data)
{
	timer2_lock();

    Locked_SetTimer2(ms_d10, proc, data);

	timer2_unlock();
}

static inline void ResetTimer2()
{
	timer2_lock();

    Locked_ResetTimer2();

	timer2_unlock();
}

static inline void KillTimer2()
{
	timer2_lock();

    Locked_KillTimer2();

	timer2_unlock();
}

void InitTimer2();

#endif//_TIMER2_H_

