#ifndef _TOS_H_
#define _TOS_H_

/////////////////////////////////////////////////////////////////////////////
//                        OS配置
/////////////////////////////////////////////////////////////////////////////

#define THREAD_NUM  8          // 线程个数, 可配置，根据实际情况配. 0=idle线程, 1=主线程

void os_init();


// 返回0, ok. 其他: 堆栈下溢的线程号
int os_stack_check(BOOL isPrint=TRUE);
void os_run();

/////////////////////////////////////////////////////////////////////////////
//                        信号量
/////////////////////////////////////////////////////////////////////////////

struct sema_t {
	int count;    // >=0, 表示信号量个数. <0, 表示等待的线程数
};

// return 0, ok. <0, err
// timeout=0, 不等待, timeout<0, 无限等待, timeout>0, 等待的时间(毫秒)
int p_sema(sema_t *, int timeout);
void v_sema(sema_t *);
#define init_sema(s, num) ((s)->count = (num))


/////////////////////////////////////////////////////////////////////////////
//                        线程
/////////////////////////////////////////////////////////////////////////////

typedef void THREAD_ENTRY(void *data);

// 返回>=0, 线程id, <0, 错误
int create_thread(THREAD_ENTRY *entry,
				  void *data,
				  U32 stack_base,
				  U32 stack_size,
				  int priority);

void sleep(int ms);

// return 0, ok. <0, err
int wait_flag(volatile int *flag, int timeout);

/////////////////////////////////////////////////////////////////////////////
//                        任务切换
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//                        错误代码
/////////////////////////////////////////////////////////////////////////////

// 错误号
#define OE_OK 0

#define OE_BASE           -21000

#define OE_TIMEOUT        (OE_BASE)

#define OE_MEMOUT         (OE_BASE - 1)

// system errors:
#define OE_SYS            (OE_BASE - 2)
#define OE_BAD_THREAD_ID  (OE_BASE - 3)
#define OE_THREAD_FULL    (OE_BASE - 4)
#define OE_THREAD_STACK_ALIGN    (OE_BASE - 5)
#define OE_THREAD_STACK_SIZE     (OE_BASE - 6)

#endif // _TOS_H_

